目标检测模型（人、人脸、人头、机动车、非机动车、宠物(猫狗)）：
object_detection_ipc_cls7.data： 输入为896x512x3
object_detection_ipc_cls7_v.data： 输入为512x896x3

目标检测（人、人脸、宠物(猫狗)）
object_detection_pfp.data: 输入为512x288x3 
object_detection_pfp_896x512.data：输入为896x512x3

注1：如果需要使用896x512x3模型，只需把文件名object_detection_pfp_896x512.data改为object_detection_pfp.data并替换即可生效

注2：如果需要选用人、人脸、宠物的检测模型，初始化时配置参数需要加上宠物类别
```
commonParams->detObjectType |= ROCKIVA_OBJECT_TYPE_PET;
commonParams->detObjectType |= ROCKIVA_OBJECT_TYPE_FACE;
commonParams->detObjectType |= ROCKIVA_OBJECT_TYPE_PERSON;
```

人脸质量评估：
face_quality_v2.data

人脸口罩识别：
face_mask_classify.data