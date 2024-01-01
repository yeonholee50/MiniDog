import cv2
import numpy as np
from tensorflow import keras
from tensorflow.keras.preprocessing import image
from tensorflow.keras.applications.resnet50 import preprocess_input, decode_predictions

# Load pre-trained ResNet50 model
model = keras.applications.resnet.ResNet50(weights='imagenet')

# Initialize camera objects
camera1 = cv2.VideoCapture(0)  # Change the index if needed
camera2 = cv2.VideoCapture(1)  # Change the index if needed

# Check if cameras opened successfully
if not camera1.isOpened() or not camera2.isOpened():
    print("Error: Unable to open cameras.")
    exit()

# Load list of building images for recognition
building_images = [
    cv2.imread('building1.jpg'),
    cv2.imread('building2.jpg'),
    # Add more building images as needed
]

# Define a function to distort the image
def distort_image(img):
    # Apply random distortions like rotation, flipping, etc.
    angle = np.random.uniform(-30, 30)  # Random rotation angle between -30 and 30 degrees
    scale = np.random.uniform(0.8, 1.2)  # Random scaling factor between 0.8 and 1.2
    flip = np.random.choice([True, False])  # Random flipping

    # Apply transformations
    rows, cols, _ = img.shape
    M = cv2.getRotationMatrix2D((cols / 2, rows / 2), angle, scale)
    img = cv2.warpAffine(img, M, (cols, rows))
    
    if flip:
        img = cv2.flip(img, 1)  # 1 means horizontal flipping

    return img

while True:
    # Read frames from cameras
    ret1, frame1 = camera1.read()
    ret2, frame2 = camera2.read()

    # Check if frames were read successfully
    if not ret1 or not ret2:
        print("Error: Unable to read frames from cameras.")
        break

    # Distort frames for better building recognition
    frame1_distorted = distort_image(frame1.copy())
    frame2_distorted = distort_image(frame2.copy())

    # Display distorted frames
    cv2.imshow('Camera 1 - Distorted', frame1_distorted)
    cv2.imshow('Camera 2 - Distorted', frame2_distorted)

    # Convert distorted frames to RGB for ResNet50 model
    img1 = cv2.cvtColor(frame1_distorted, cv2.COLOR_BGR2RGB)
    img2 = cv2.cvtColor(frame2_distorted, cv2.COLOR_BGR2RGB)

    # Resize images to the required input size for ResNet50 (224x224)
    img1 = cv2.resize(img1, (224, 224))
    img2 = cv2.resize(img2, (224, 224))

    # Preprocess images for ResNet50 model
    img1 = image.img_to_array(img1)
    img1 = np.expand_dims(img1, axis=0)
    img1 = preprocess_input(img1)

    img2 = image.img_to_array(img2)
    img2 = np.expand_dims(img2, axis=0)
    img2 = preprocess_input(img2)

    # Predict building using ResNet50 model
    predictions1 = model.predict(img1)
    predictions2 = model.predict(img2)

    # Decode predictions to get class labels
    label1 = decode_predictions(predictions1)[0][0][1]
    label2 = decode_predictions(predictions2)[0][0][1]

    # Display recognized building labels
    print("Camera 1 - Recognized Building:", label1)
    print("Camera 2 - Recognized Building:", label2)

    # Calculate disparity between corresponding points
    sift = cv2.SIFT_create()
    kp1, des1 = sift.detectAndCompute(frame1_distorted, None)
    kp2, des2 = sift.detectAndCompute(frame2_distorted, None)

    bf = cv2.BFMatcher()
    matches = bf.knnMatch(des1, des2, k=2)

    # Apply ratio test
    good_matches = []
    for m, n in matches:
        if m.distance < 0.75 * n.distance:
            good_matches.append(m)

    # Calculate average disparity
    disparity_sum = 0
    for match in good_matches:
        pt1 = kp1[match.queryIdx].pt
        pt2 = kp2[match.trainIdx].pt
        disparity_sum += abs(pt1[0] - pt2[0])

    if len(good_matches) > 0:
        average_disparity = disparity_sum / len(good_matches)
        print("Average Disparity:", average_disparity)

        # Use a simple linear relation for distance estimation
        # You may need to calibrate this based on your setup
        distance = 100 / average_disparity  # Adjust the scaling factor as needed
        print("Estimated Distance to Building:", distance)

    # Break the loop if 'q' key is pressed
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release camera objects and close windows
camera1.release()
camera2.release()
cv2.destroyAllWindows()
