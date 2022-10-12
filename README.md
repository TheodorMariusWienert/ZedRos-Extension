This work is an extension of the [zed-ros-wrapper](https://github.com/stereolabs/zed-ros-wrapper).
It is now possible to use a custom [YOLOv5](https://github.com/ultralytics/yolov5) Model to detect all classes the YOLOv5 model was trained on.
It archieves that by using the [YOLOv5-TensorRT](https://github.com/noahmr/yolov5-tensorrt), many thanks to the author.

How the extension was implemented is based on [ZED-YOLOv5]( https://github.com/noahmr/zed-yolov5) and [Yolov5 with ZED Custom Box input](https://github.com/stereolabs/zed-examples/tree/master/object%20detection/custom%20detector/cpp/tensorrt_yolov5_v6.0=



To install this package you need first to install the following libaries:

##Libraries needed
- Cuda >10.02
- Tensorrt  > 8
- OpenCV (when possible wiht CUDA)
- yolov5-tensorrt
- ZED SDK
- Ros Noetic
## It was tested with these libaries instaled
- Cuda 11.7
- Tensorrt  8.4.2-1
- OpenCV with Cuda
- yolov5-tensorrt
- ZED SDK 3.7 version
- Ros Noetic

And a ZED2 was used. Further a YOLOv5s model was use as a FP16 and FP32 TensorRT Engine.

To install this project you need to do the following (analog to [instructions](https://www.stereolabs.com/docs/ros/) given by stereolabs):

  cd ~/catkin_ws/src
  git clone --recursive https://github.com/TheodorMariusWienert/ZedRos-Extension
  cd ../
  rosdep install --from-paths src --ignore-src -r -y    # This gives some errors sometime but can be ignored if they dont affect the code
  catkin_make -DCMAKE_BUILD_TYPE=Release
  source ./devel/setup.bash


Now you need to modify the zed2.yaml and place the path to your TensorRT engine in the file. And specify to use a custom detector. Also you can enter the names of the classes you want to detect here.

To use your custom Yolov5 model you need to either export it to a FP32 engine with the export functionality of [YOLOv5](https://github.com/ultralytics/yolov5). Or when you want to use a FP16 engine you need to first export it to Onnx format with  [YOLOv5](https://github.com/ultralytics/yolov5) and than use the [YOLOv5-TensorRT](https://github.com/noahmr/yolov5-tensorrt) functionality to export it to a FP16 TensorRT engine.


To start the programm  use:
$ roslaunch zed_wrapper zed2.launch
For visulaization use:
$ roslaunch zed_display_rviz display_zed2.launch

##For Time recording
To record times you need to eddit zed_wrapper_nodelet.hpp and edit the pathFile and also set time recording to true
To record times at subscriber site you need to edit the  zed_obj_det_sub and change the bools to true also set the path according to your paths
To process the objects a numerous of Jupyter notebooks are in the Jupyter notebooks folder

To use GPU MATS use the branch GPU Mat, CV with CUDA needs to be installed, this was not tested really much use with care
