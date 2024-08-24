import torch
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as f
from torch.utils.data import DataLoader

from torchvision import datasets, transforms

import numpy as np
import argparse

class Convol(nn.Module):
    def __init__(self, lr, out_dims, fc1_dims, fc2_dims) -> None:
        super(Convol, self).__init__()
        self.fc1_dims = fc1_dims
        self.fc2_dims = fc2_dims
        self.out_dims = out_dims
        self.lr = lr
        
        self.conv1 = nn.Conv2d(in_channels=1, out_channels=32, kernel_size=6, stride=2)
        self.pool1 = nn.MaxPool2d(kernel_size=2, stride=2)
        self.conv2 = nn.Conv2d(in_channels=32, out_channels=64, kernel_size=3, stride=1)
        self.pool2 = nn.MaxPool2d(kernel_size=2, stride=2)
        self.f1 = nn.Linear(64*2*2, fc1_dims)
        self.f2 = nn.Linear(fc1_dims, out_dims)
        
        self.opt = optim.Adam(params=self.parameters(), lr=self.lr)
        self.lossfn = nn.CrossEntropyLoss()
        self.device = ('cuda' if torch.cuda.is_available() else 'cpu')
        self.to(device=self.device)

    def forward(self, input):
        x = f.relu(self.conv1(input))
        x = self.pool1(x)
        x = f.relu(self.conv2(x))
        x = self.pool2(x)
        x = torch.flatten(x, start_dim=1)
        x = f.relu(self.f1(x))
        return self.f2(x)
    
class Model():
    def __init__(self, lr, batch_size, out_dims, layer_dims=64):
        self.lr = lr
        self.batch_size = batch_size
        self.out_dims = out_dims
        self.layer_dims = layer_dims
        self.model = Convol(lr=self.lr, out_dims=self.out_dims, fc1_dims=self.layer_dims, fc2_dims=self.layer_dims)

    def train_model(self, epochs, train_dataset, test_dataset, verbose=True, skip_test=False):
        train_data_loader = DataLoader(dataset=train_dataset, batch_size=self.batch_size, shuffle=True, pin_memory=True)

        for epo in range(epochs):
            self.model.train()
            running_loss = 0.0
            for batch_id, (x_dat, y_dat) in enumerate(train_data_loader):
                x_dat, y_dat = x_dat.to(self.model.device, non_blocking=True), y_dat.to(self.model.device, non_blocking=True)
                self.model.opt.zero_grad()
                y_pred = self.model(x_dat)
                loss = self.model.lossfn(y_pred, y_dat)
                loss.backward()
                self.model.opt.step()
                running_loss += loss.item()
                if verbose and batch_id % 100 == 0:
                    print(f'Train Epoch: {epo+1} [{batch_id * self.batch_size}/{len(train_dataset)}] ({(100. * batch_id / len(train_data_loader)):.2f}%)\t{loss:.4f}')
            
            print("")
            del x_dat, y_dat, y_pred, loss
            if not skip_test:
                self.evaluate_model(test_dataset)

    
    def evaluate_model(self, dataset):
        test_data_loader = DataLoader(dataset=dataset, batch_size=32, shuffle=False, pin_memory=True)
        self.model.eval()

        correct = 0
        running_loss = 0.0
        with torch.no_grad():
            for x_dat, y_dat in test_data_loader:
                x_dat, y_dat = x_dat.to(self.model.device, non_blocking=True), y_dat.to(self.model.device, non_blocking=True)
                y_pred = self.model(x_dat)
                loss = self.model.lossfn(y_pred, y_dat)
                running_loss += loss.item()
                correct += (torch.argmax(y_pred, dim=1) == y_dat).sum().item()
        accuracy = correct / len(dataset)
        avg_loss = running_loss / len(test_data_loader)
        print(f"Model accuracy: {accuracy}, Loss: {avg_loss:.4f}")
        print("")
        del correct, x_dat, y_dat, loss, running_loss, accuracy, avg_loss
    
    def reset_model(self):
        del self.model
        self.model = Convol(lr=self.lr, out_dims=self.out_dims, fc1_dims=self.layer_dims, fc2_dims=self.layer_dims)

    def save_model(self):
        self.model.eval()
        dummy_input = torch.randn(1, 1, 28, 28).to(device=self.model.device)
        onnx_model_path = "saved_model.onnx"
        torch.onnx.export(
            self.model, dummy_input, onnx_model_path,
            export_params=True,
            opset_version=11,
            do_constant_folding=True,
            input_names=['input'],
            output_names=['output']
        )
        print("Model is saved successfully---------")

def dataPrep():
    transform = transforms.Compose([
        transforms.ToTensor(),
        transforms.Normalize((0.1307,), (0.3081,))
    ])

    train = datasets.MNIST(root='data',
                                train=True,
                                transform=transform,
                                download=True)
    
    test = datasets.MNIST(root='data',
                               train=False,
                               transform=transform,
                               download=True)
    return train, test

def main():
    parser = argparse.ArgumentParser(description="Hyperparameter for the model")

    #Set the arguments
    parser.add_argument('--verbose', action='store_true', help='Enable verbose output')
    parser.add_argument('--batch-size', type=int, default='32', help='Set the batch_size of the neural')
    parser.add_argument('--lr', type=float, default='0.001', help='Set the learning rate of the model')
    parser.add_argument('--epochs', type=int, default='10', help='Set the training epochs of the model')
    parser.add_argument('--skip-test', action='store_true', help='Enable skipping the evaluation phase')

    #Parse the argument
    args = parser.parse_args()

    #Prepare the dataset
    train_data, test_data = dataPrep()

    #Main process
    model = Model(lr=args.lr, batch_size=args.batch_size,
                  out_dims=10)
    
    print("--------Training the model--------")
    print("Enabling verbose:", args.verbose)
    model.train_model(args.epochs, train_data, test_data, args.verbose, args.skip_test)

    if not args.skip_test:
        print("--------Evaluating the model--------")
        model.evaluate_model(test_data)

    print("--------Saving the model--------")
    model.save_model()

if __name__ == "__main__":
    main()