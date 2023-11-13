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
#         # 평균 이미지 계산
#         avg_image = torch.mean(images, dim=0, keepdim=True)

#         # 예측 수행
#         output = model(avg_image)
#         _, predicted = torch.max(output, 1)
#         return predicted.item()

# # 이미지 변환 정의
# transform = transforms.Compose([
#     transforms.Resize((224, 224)),
#     transforms.ToTensor(),
# ])

# # 모델 정의 및 로드
# #model = EfficientNet.from_pretrained('efficientnet-b0.pth', num_classes=3)
# model = EfficientNet.from_name('efficientnet-b0', num_classes=3)
# #model.load_state_dict(torch.load(r'app\model\model_epo5.pth'))
# model = torch.load('app\model\model_epo5.pth', map_location=torch.device('cpu'))
# model.to(torch.device("cuda" if torch.cuda.is_available() else "cpu"))

# # 인자로부터 이미지 경로 로드
# image_paths = sys.argv[1:]  # 첫 번째 인자는 스크립트 이름이므로 제외
# if len(image_paths) != 13:
#     raise ValueError("Exactly 13 images are required.")

# # 이미지 로드 및 전처리
# images = load_and_preprocess_images(image_paths, transform)

# # 예측 수행
# prediction = predict(model, images)
# print("Prediction:", prediction)

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
        predictions = []
        for image in images:
            output = model(image.unsqueeze(0))
            _, predicted = torch.max(output, 1)
            predictions.append(predicted.item())
        return predictions

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
predictions = predict(model, images)
print("Predictions:", predictions)

