# import sys
# import torch
# from torchvision import transforms
# from efficientnet_pytorch import EfficientNet
# from PIL import Image

# def load_and_preprocess_images(image_paths, transform):
#     images = []
#     for path in image_paths:
#         image = Image.open(path).convert('RGB')
#         if transform:
#             image = transform(image)
#         images.append(image)
#     images = torch.stack(images)
#     return images

# def predict(model, images):
#     model.eval()
#     with torch.no_grad():
#         predictions = []
#         for image in images:
#             output = model(image.unsqueeze(0))
#             _, predicted = torch.max(output, 1)
#             predictions.append(predicted.item())
#         return predictions

# # Image transformation definition
# transform = transforms.Compose([
#     transforms.Resize((224, 224)),
#     transforms.ToTensor(),
#     transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225]),
# ])

# # Model definition and loading
# model = EfficientNet.from_name('efficientnet-b0', num_classes=3)
# model.load_state_dict(torch.load('model\\model_epo5.pth', map_location=torch.device('cpu')))
# model.to(torch.device("cuda" if torch.cuda.is_available() else "cpu"))

# # Arguments for image paths
# image_paths = sys.argv[1:]
# if len(image_paths) != 13:
#     raise ValueError("Exactly 13 images are required.")

# # Load and preprocess images
# images = load_and_preprocess_images(image_paths, transform)

# # Perform prediction
# predictions = predict(model, images)
# print("Predictions:", predictions)

import sys
import torch
from torchvision import transforms
from efficientnet_pytorch import EfficientNet
from PIL import Image

def load_and_preprocess_images(image_paths, transform):
    images = []
    for path in image_paths:
        image = Image.open(path).convert('RGB')
        if transform:
            image = transform(image)
        images.append(image)
    images = torch.stack(images)
    return images

def predict(model, images):
    model.eval()
    with torch.no_grad():
        # Instead of predicting for each image, we predict for all and average the outputs
        outputs = torch.zeros([len(images), 3])  # Assuming 3 classes as in the original script
        for i, image in enumerate(images):
            output = model(image.unsqueeze(0))
            outputs[i] = torch.nn.functional.softmax(output, dim=1)

        # Averaging the predictions
        avg_output = outputs.mean(dim=0)
        _, predicted = torch.max(avg_output, 0)
        return predicted.item()

# Image transformation definition
transform = transforms.Compose([
    transforms.Resize((224, 224)),
    transforms.ToTensor(),
    transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225]),
])

# Model definition and loading
model = EfficientNet.from_name('efficientnet-b0', num_classes=3)
model.load_state_dict(torch.load('model\\model_epo5.pth', map_location=torch.device('cpu')))
model.to(torch.device("cuda" if torch.cuda.is_available() else "cpu"))

# Arguments for image paths
image_paths = sys.argv[1:]
if len(image_paths) != 13:
    raise ValueError("Exactly 13 images are required.")

# Load and preprocess images
images = load_and_preprocess_images(image_paths, transform)

# Perform prediction
prediction = predict(model, images)

# Map the predicted index to the corresponding label
class_labels = ["Ideal", "Low", "Overweight"]
predicted_label = class_labels[prediction]

print(predicted_label)
