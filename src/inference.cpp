#include "inference.h"

#include <iostream>
#include <onnxruntime/core/session/onnxruntime_cxx_api.h>
#include <wx/wx.h>

ModelInference::ModelInference()
{   
    //Define the environment
    auto env_local = std::make_unique<Ort::Env>(ORT_LOGGING_LEVEL_WARNING, "test");
    env = std::move(env_local);

    //Initialize the model path
    model_path = L"../model/saved_model.onnx";

    //Define a new session
    auto session_local = std::make_unique<Ort::Session>(*env, model_path, Ort::SessionOptions{nullptr});
    session = move(session_local);

    //Define the run options
    run_options = Ort::RunOptions();
};

static std::pair<float, int> argmax(void * p_data, size_t p_data_len){
    //Cast the generic type into a specific data type
    float *data = static_cast<float*>(p_data);
    int len = static_cast<int>(p_data_len);

    //Find the output with the highes probs
    std::pair<float, int> max_val = {data[0], 0};
    for(int i = 1; i < len; i++){
        if(data[i] > max_val.first){
            max_val = {data[i], i};
        }
    }

    //Return the maximum value
    return max_val;
}

float normalizer(const float& input, const float& mean, const float& std){
    return (input - mean) / std;
}

void ModelInference::LoadModel(const std::string& path){
    model_path = std::wstring(path.begin(), path.end()).c_str();
}

int ModelInference::RunInference(float** arr, const int& width, const int& height){
    //Define the data shape
    const std::array<int64_t, 4> input_shape = {1, numChannels, height, width};
    const std::array<int64_t, 2> output_shape = {1, numClasses};

    //Define the array
    std::array<float, numInputElements> input;
    std::array<float, numClasses> result;


    //Define the tensor
    auto memory_info = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);
    auto input_tensor = Ort::Value::CreateTensor<float>(memory_info, input.data(), input.size(), input_shape.data(), input_shape.size());
    auto output_tensor = Ort::Value::CreateTensor<float>(memory_info, result.data(), result.size(), output_shape.data(), output_shape.size());

    //Copy and normalize the data to the input array
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            input[i*width+j] = normalizer((float)arr[j][i], 0.1307f, 0.3081f);
        }
    }

    //Get the input and output names
    Ort::AllocatorWithDefaultOptions ort_alloc;
    Ort::AllocatedStringPtr input_name = session->GetInputNameAllocated(0, ort_alloc);
    Ort::AllocatedStringPtr output_name = session->GetOutputNameAllocated(0, ort_alloc);
    const std::array<const char*, 1> input_names = {input_name.get()};
    const std::array<const char*, 1> output_names = {output_name.get()};

    //const std::array<const char*, 1> input_names = {"input"};
    //const std::array<const char*, 1> output_names = {"output"};

    //Run the model inference
    try{
        session->Run(run_options, input_names.data(),
                    &input_tensor, 1,
                    output_names.data(),
                    &output_tensor, 1);
    }
    catch(Ort::Exception& e){
        wxLogMessage("OrtError: %s", e.what());
        return -1;
    }

    //Find the prediction with the maximum probability
    std::pair<float, int> pred = argmax(result.data(), result.size());

    //Return the prediction
    return pred.second;
}