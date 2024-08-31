#pragma once

#include <onnxruntime/core/session/onnxruntime_cxx_api.h>

class ModelInference{
public:
    ModelInference();
    int RunInference(float **, const int&, const int&);
    void LoadModel(const std::string&);
    ~ModelInference(){
    }
private:
    std::unique_ptr<Ort::Session> session = nullptr;
    std::unique_ptr<Ort::Env> env = nullptr;
    Ort::RunOptions run_options;

    const wchar_t *model_path;
    
    static constexpr int64_t numChannels = 1;
    static constexpr int64_t width = 28;
    static constexpr int64_t height = 28;
    static constexpr int64_t numClasses = 10;
    static constexpr int64_t numInputElements = numChannels * width * height;
};