#ifndef __PANORAMA_STITCHINGAPP_H__
#define __PANORAMA_STITCHINGAPP_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


#if defined _WIN32 || defined __CYGWIN__
#ifdef BUILDING_DLL
#ifdef __GNUC__
#define DLL_PUBLIC __attribute__ ((dllexport))
#else
	// Note: actually gcc seems to also supports this syntax.
#define DLL_PUBLIC __declspec(dllexport)
#endif
#else
#ifdef __GNUC__
#define DLL_PUBLIC __attribute__ ((dllimport))
#else
	// Note: actually gcc seems to also supports this syntax.
#define DLL_PUBLIC
#endif
#define DLL_LOCAL
#endif
#else
#if __GNUC__ >= 4
#define DLL_PUBLIC __attribute__ ((visibility ("default")))
#define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define DLL_PUBLIC
#define DLL_LOCAL
#endif
#endif

	/*
	 * @enum    RK_PS_RETURN_STATUS
	 * @brief   the return status of function
	 */
	enum RK_PS_RETURN_STATUS
	{
		RK_PS_STATUS_EOF = -1,			/**< error of function inside */
		RK_PS_STATUS_OK = 0,			/**< run successfully */
		RK_PS_STATUS_FILE_READ_ERROR,	/**< error: fail to read file */
		RK_PS_STATUS_FILE_WRITE_ERROR,	/**< error: fail to write file */
		RK_PS_STATUS_INVALID_PARAM,		/**< error: invalid parameter */
		RK_PS_STATUS_ALLOC_FAILED,		/**< error: fail to alloc buffer */
		AVS_STATUS_BUTT					/**< error: reserved fields */
	};

	/*
	 * @enum    RK_PS_MESH_ALPHA_SAVE_OPTION
	 * @brief   Specify the way of saving mesh and alpha data
	 */
	enum RK_PS_MESH_ALPHA_SAVE_OPTION
	{
		WRITE_MESH_ALPHA_BUFFER = 0,			/**< write mesh and alpha in buffer */
		WRITE_MESH_ALPHA_BIN_FILE = 1,			/**< write mesh and alpha as bin file */
	};

	/*
	 * @enum    RK_PS_LOW_FREQUENCY_LAYER_FUSE_WIDTH
	 * @brief   Specify the low frequency layer fuse width
	 */
	enum RK_PS_LOW_FREQUENCY_LAYER_FUSE_WIDTH
	{
		LOW_FREQUENCY_LAYER_FUSE_WIDTH_128 = 128,	/**< fuse_width = 128 */
		LOW_FREQUENCY_LAYER_FUSE_WIDTH_256 = 256,	/**< fuse_width = 256 */
		LOW_FREQUENCY_LAYER_FUSE_WIDTH_512 = 512,	/**< fuse_width = 512 */
	};

	/*
	 * @enum    RK_PS_IMAGE_FORMAT
	 * @brief   Specify the input images format
	 */
	enum RK_PS_IMAGE_FORMAT
	{
		INPUT_FORMAT_TYPE_GRAY = 0,				/**< input gray images */
		INPUT_FORMAT_TYPE_RGB = 1,				/**< input rgb images */
		INPUT_FORMAT_TYPE_YUVNV12 = 2,			/**< input yuv_nv12 images */
	};

	/*
	 * @enum    RK_PS_SAVE_MESH_ALPHA_TYPE
	 * @brief   Specify the save mesh and alpha data type
	 */
	enum RK_PS_SAVE_MESH_ALPHA_TYPE
	{
		SAVE_MESH_ALPHA_TYPE_GPU = 0,				/**< output rk mesh alpha data for gpu(3588) */
		SAVE_MESH_ALPHA_TYPE_CMODEL = 1,			/**< output rk mesh alpha data for cmodel */
		SAVE_MESH_ALPHA_TYPE_DEBUG = 2,				/**< output rk mesh alpha data for debug */
		SAVE_MESH_ALPHA_TYPE_FEC = 3,				/**< output rk mesh alpha data for FEC */
	};

	/*
	 * @enum    RK_PS_CAMERA_TYPE
	 * @brief   Specify the camera type
	 */
	enum RK_PS_CAMERA_TYPE
	{
		RK_PS_CAMERA_TYPE_PINHOLE,				/**< camera pinhole type */
		RK_PS_CAMERA_TYPE_OMNI,					/**< camera omnidirectional type */
		RK_PS_CAMERA_TYPE_FISH,					/**< camera fisheye type */
	};

	/*
	 * @enum    RK_PS_PROJECT_TYPE
	 * @brief   Specify the project type
	 */
	enum RK_PS_PROJECT_TYPE
	{
		RK_PS_PROJECTION_EQUIRECTANGULAR = 0,	/**< equirectangular */
		RK_PS_PROJECTION_RECTILINEAR = 1,		/**< rectilinear */
		RK_PS_PROJECTION_CYLINDRICAL = 2,		/**< cylindrical */
		RK_PS_PROJECTION_CUBE_MAP = 3,			/**< cube_map: currently unsupported */
	};

	/*
	 * @enum    RK_PS_SrcOverlapPosition
	 * @brief   Clockwise rotation angle of overlapping area from src image to dst image
	 */
	enum RK_PS_SrcOverlapPosition
	{
		RK_PS_SrcOverlapPosition_0 = 0,			/**< clockwise rotation 0 degrees */
		RK_PS_SrcOverlapPosition_90 = 1,		/**< clockwise rotation 90 degrees */
		RK_PS_SrcOverlapPosition_180 = 2,		/**< clockwise rotation 180 degrees */
		RK_PS_SrcOverlapPosition_270 = 3,		/**< clockwise rotation 270 degrees */
	};

	/*
	 * @struct  RK_PS_SrcOverlapMap
	 * @brief   the overlap map data and position on src image
	 */
	struct RK_PS_SrcOverlapMap
	{
		char versionInfo[64];
		RK_PS_SrcOverlapPosition srcOverlapPositon[8];
		unsigned char overlapMap[15 * 15 * 8];
	};

	struct RK_PS_MeshParams
	{
		int nonoverlap_num;
		int overlap_num;
		int nonoverlap_dst_startx[2];
		int nonoverlap_dst_width[2];
		int nonoverlap_mesh_width[2];
		int nonoverlap_mesh_height[2];

		int overlap_band_num;
		int overlap_dst_startx[2];
		int overlap_dst_width[2];
		int overlap_mesh_width[2];
		int overlap_mesh_height[2];
		int overlap_alpha_width[2];
		int overlap_alpha_height[2];
	};
	/*
	 * @struct  RK_PS_MeshBuffer
	 * @brief   output buffer for mesh
	 */
	struct RK_PS_MeshBuffer
	{
		int src_width;
		int src_height;
		int dst_width;
		int dst_height;
		int mesh_step_x;
		int mesh_step_y;
		int camera_num;
		float* pmesh;
		RK_PS_MeshParams mesh_params[8];
	};

	/*
	 * @struct  RK_PS_AlphaBuffer
	 * @brief   output buffer for alpha
	 */
	struct RK_PS_AlphaBuffer
	{
        void* place_holder;
	};

	/*
	 * @struct  RK_PS_Engine
	 * @brief   the internal engine instance
	 */
	struct RK_PS_Engine
	{
		RK_PS_MeshBuffer* mesh;					/**< input/output-mesh buffer */
		RK_PS_AlphaBuffer* alpha;				/**< input/output-alpha buffer */
		void* p;
	};

	/*
	 * @struct  RK_PS_InitParams
	 * @brief   params for initialize calibration data
	 */
	struct RK_PS_InitParams
	{
		RK_PS_MESH_ALPHA_SAVE_OPTION save_option;	/**< input-specify save option */
		int camera_num;								/**< input-camera numbers */
		const char* input_calib_data_path;			/**< input-cameras calibration data path */
		const char* input_mask_data_path;			/**< input-mask image path */
		const char* input_lut_path;					/**< input-specify src LUT file path */
		const char* output_mesh_alpha_data_path;	/**< output-mesh and alpha data path */
		const char* output_overlap_data_path;		/**< output-overlap data path */
		int mesh_step_x;							/**< input-mesh step in horizontal direction */
		int mesh_step_y;							/**< input-mesh step in vertical direction */
		int fuse_width;								/**< input-min width of fuse band: 128 or 256 or 512 */
	};

	/*
	 * @struct  RK_PS_Rotation
	 * @brief   rotation angle
	 */
	struct RK_PS_Rotation
	{
		int yaw100;								/**< yaw data. yaw angle x 100 */
		int pitch100;							/**< pitch data. pitch angle x 100 */
		int roll100;							/**< roll data. roll angle x 100 */
	};

	/*
	 * @struct  RK_PS_SetProjectParams
	 * @brief   params for the specifications of the output image
	 */
	struct RK_PS_SetProjectParams
	{
		int project_type;						/**< input-project type */
		int camera_num;							/**< input-cameras number */
		int src_width;							/**< input-input_images width */
		int src_height;							/**< input-input_images height */
		int dst_width;							/**< input-output stitch image width */
		int dst_height;							/**< input-output stitch image height */
		int fov_x100;							/**< input-output stitch image fov_x */
		int fov_y100;							/**< input-output stitch image fov_y */
		int center_x;							/**< input-output stitch image center_y */
		int center_y;							/**< input-output stitch image center_y */
		float stitch_distance;					/**< input-best stitch distance */
		int save_mesh_alpha_type;				/**< input-output mesh and alpha type */
		RK_PS_Rotation ori_rotation;			/**< input-output stitch image ori rotation */
		RK_PS_Rotation rotation;				/**< input-output stitch image rotation */
	};

	/*
	 * @struct  RK_PS_FineTuningParams
	 * @brief   fine tuning params for every camera
	 */
	struct RK_PS_FineTuningParams
	{
		int camera_num;
		int offset_h[8];
		int offset_w[8];
		int offset_z[8];
		RK_PS_Rotation rotation[8];
	};

	/*
	 * @struct  RK_PS_Roi_Params
	 * @brief   params for roi
	 */
	struct RK_PS_RoiParams
	{
		int src_project_type;					/**< input-project type */
		int src_width;							/**< input-input_image width */
		int src_height;							/**< input-input_image height */
		int src_fov_x100;						/**< input-input_image fov_x */
		int src_fov_y100;						/**< input-input_image fov_y */
		int src_center_x;
		int src_center_y;

		int dst_width;							/**< input-output stitch image width */
		int dst_height;							/**< input-output stitch image height */
		int dst_fov_x100;						/**< input-output stitch image fov_x */
		int dst_fov_y100;						/**< input-output stitch image fov_y */
		int dst_center_x;						/**< input-output stitch image center_y */
		int dst_center_y;						/**< input-output stitch image center_y */
	};

	/*
	 * @struct  RK_PS_InitBlendedImages
	 * @brief   params for initialize blended images
	 */
	struct RK_PS_InitBlendedImages
	{
		int camera_num;							/**< input-cameras number */
		int image_format;						/**< input-images format */
		int use_gain;							/**< input-use gain or not(0-not use,1-use) */
		int gain_stepx;							/**< input-gian step_x */
		int gain_stepy;							/**< input-gian step_y */
		int input_mesh_alpha_data_type;			/**< input-input mesh and alpha data type */
		const char* input_mesh_alpha_data_path;	/**< input-input mesh and alpha data path */
		unsigned char* output_blended_image;	/**< output-output blended image */
	};

	/* 创建Engine的函数 */
	DLL_PUBLIC int RK_PS_setEngine(struct RK_PS_Engine* engine);

	/* 生成mesh表和alpha表时的初始化标定参数的函数 */
	DLL_PUBLIC int RK_PS_initParams(struct RK_PS_Engine* engine, RK_PS_InitParams* params);

	/* 生成mesh表和alpha表时设置输出图像规格的函数 */
	DLL_PUBLIC int RK_PS_setProjectParams(struct RK_PS_Engine* engine, struct RK_PS_SetProjectParams* params);

	/* 生成mesh表和alpha表时设置每个相机姿态精调参数的函数 */
	DLL_PUBLIC int RK_PS_setFineTuningParams(struct RK_PS_Engine* engine, struct RK_PS_FineTuningParams* params);

	//获取GPU所需查找表所需的最大buffer
	DLL_PUBLIC int RK_PS_getMaxMeshBufferSize(struct RK_PS_Engine* engine, float* buffer_size);

	/* 通过标定文件生成mesh表和alpha表的函数 */
	DLL_PUBLIC int RK_PS_mesh_alphaFromCalibration(struct RK_PS_Engine* engine);
	
	/* 通过标定文件计算输入图像重叠区域mask信息 */
	DLL_PUBLIC int RK_PS_srcOverlapMaskFromCalibration(struct RK_PS_Engine* engine);

	/* 通过中间LUT表生成mesh表和alpha表的函数 */
	DLL_PUBLIC int RK_PS_mesh_alphaFromProjection(struct RK_PS_Engine* engine);

	/* 通过用户指定的ROI参数生成rectilinear校正的mesh表的函数 */
	DLL_PUBLIC int RK_PS_meshForRoiRectilinear(struct RK_PS_Engine* engine, RK_PS_RoiParams* roi_parmas, const char* save_mesh_path);

	/* 拼接图像的初始化函数 */
	DLL_PUBLIC int RK_PS_initBlendedImages(struct RK_PS_Engine* engine, struct RK_PS_InitBlendedImages* params);

	/* 拼接图像函数主函数 */
	DLL_PUBLIC int RK_PS_blendedImages(struct RK_PS_Engine* engine, unsigned char** pinput_image);

	/* 析构函数 */
	DLL_PUBLIC int RK_PS_destroy(struct RK_PS_Engine* engine);

#ifdef __cplusplus
} /* extern "C" { */
#endif




#endif // !__PANORAMA_STITCHINGAPP_H__
