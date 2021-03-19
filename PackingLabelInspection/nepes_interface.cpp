/* nepes corp.
   JHLEE 2020-06-29 Start
*/

#pragma once

#include "nepes_interface.hpp"
#include "string.h"
#include "libxl.h"

#include <tchar.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include <windows.h>
#include <time.h>
#include <algorithm>

#include <cpprest/http_client.h>


#ifdef _DEBUG
#define _DEBUG_WAS_DEFINED 1
#undef _DEBUG
#endif

#include <Python.h>

#ifdef _DEBUG_WAS_DEFINED
#define _DEBUG 1
#endif

enum class MESCommonKey
{
	NEPES_MES_KEY_STATUS,
	NEPES_MES_KEY_MESSAGE,
	NEPES_MES_KEY_RETURN_VALUE,
	NEPES_MES_KEY_RECORDS,
};

enum class MESRecipeTypeKey
{
	NEPES_MES_KEY_SIDE_TYPE,
	NEPES_MES_KEY_RECIPE,
	NEPES_MES_KEY_CUSTOMER,
};

enum class MESRecipeInformationKey
{
	NEPES_MES_KEY_SPEC_ID,
	NEPES_MES_KEY_EXCEPTION,
	NEPES_MES_KEY_ITEM_TYPE,
	NEPES_MES_KEY_TOP_LEFT_Y,
	NEPES_MES_KEY_BOTTOM_RIGHT_X,
	NEPES_MES_KEY_TOP_LEFT_X,
	NEPES_MES_KEY_BOTTOM_LEFT_X,
	NEPES_MES_KEY_BOTTOM_RIGHT_Y,
	NEPES_MES_KEY_ITEM_SEQ,
	NEPES_MES_KEY_CONTENT,
	NEPES_MES_KEY_ITEM_ID,
	NEPES_MES_KEY_ITEM_NAME,
	NEPES_MES_KEY_LABEL_ID,
	NEPES_MES_KEY_TOP_RIGHT_X,
	NEPES_MES_KEY_TOP_RIGHT_Y,
	NEPES_MES_KEY_ROI_NUMBER,
	NEPES_MES_KEY_REEL_NUMBER,
	NEPES_MES_KEY_LABEL_SEQ,
	NEPES_MES_KEY_LABEL_NAME,
	NEPES_MES_KEY_BOTTOM_Y,
};

enum class MESParameterName
{
	NEPES_MES_PARAMETER_NAME_PLANT,
	NEPES_MES_PARAMETER_NAME_TRANSACTION,
	NEPES_MES_PARAMETER_NAME_ACTION_TYPE,
	NEPES_MES_PARAMETER_NAME_LOT_NUMBER,
	NEPES_MES_PARAMETER_NAME_USER,
	NEPES_MES_PARAMETER_NAME_RESULT,
	NEPES_MES_PARAMETER_NAME_OPERATION,
	NEPES_MES_PARAMETER_NAME_EQUIPMENT,
};

enum class MESConstParameterValue
{
	NEPES_MES_PARAMETER_VALUE_PLANT_TEST,
	NEPES_MES_PARAMETER_VALUE_PLANT_RUNNING,
	NEPES_MES_PARAMETER_VALUE_TRANSACTION_SPIN,
	NEPES_MES_PARAMETER_VALUE_TRANSACTION_SPOU,
	NEPES_MES_PARAMETER_VALUE_ACTION_TYPE_GET_RECIPE_TYPE,
	NEPES_MES_PARAMETER_VALUE_ACTION_TYPE_GET_PISP_DATA,
	NEPES_MES_PARAMETER_VALUE_ACTION_TYPE_SAVE_INFERENCE_RESULT,
	NEPES_MES_PARAMETER_VALUE_USER,
	NEPES_MES_PARAMETER_VALUE_OPERATION,
};

enum class AIResultType
{
	NEPES_AI_RESULT_PASS,
	NEPES_AI_RESULT_FAIL_READING,
	NEPES_AI_RESULT_FAIL_MATCHING,
	NEPES_AI_RESULT_TOTAL,
};

struct JudgeInfo {
	int object_id;
	double degree;
	double distance;
	NepesInterface::RectanglePosition position;
};

struct WrapperMESResponseSet {
	std::string status;
	std::string result;
};

struct WrapperModelInformation {
	std::string customer;
	std::string recipe;
	NepesInterface::ModelType type;
};

struct WrapperBarcodeDataset {
	NepesInterface::Vertex center_position;
	std::string barcode_reading_data;
};

struct WrapperVisionROIInformation {
	std::string roi_name;
	NepesInterface::RectanglePosition roi_region;
};

struct MESLabelInformation {
	bool exception;
	NepesInterface::RectanglePosition teaching_position;
	std::string content;
	std::string roi_name;
	int reel_number;
	int label_seq;
	std::string label_name;
	//2021-02-22 JHLEE Item name 추가
	std::string item_name;
};

struct CurrentTextInformation {
	int object_id;
	int reel_no;
	int label_no;
	NepesInterface::RectanglePosition bounding_boxes;
	NepesInterface::Vertex center_position;
	std::string text_data;
	std::string image_name;
	//2021-02-22 JHLEE Item name 추가
	std::string item_name;
	bool comparison;
	bool exception;
};

struct CurrentBarcodeInformation {
	int object_id;
	int reel_no;
	int label_no;
	NepesInterface::RectanglePosition bounding_boxes;
	NepesInterface::Vertex center_position;
	std::string barcode_data;
	std::string image_name;
	bool comparison;
	bool exception;
};

//2020-11-23 JHLEE
//고과장님 요청사항 추가
struct DetailDetectionStruct {
	CurrentTextInformation current_text_info;	//MES 기준 정보 기준으로 묶은 텍스트 정보
	std::vector<CurrentTextInformation> rct_vector;	//묶기 전 AI Detection Model이 Return한 Position들
};

std::string g_image_path;

std::string g_exception_label_name;

std::vector<NepesInterface::RectanglePosition> g_detection_result;

std::vector<CurrentTextInformation> g_current_text_info;
std::vector<CurrentBarcodeInformation> g_barcode_teaching_info;
std::vector<CurrentTextInformation> g_object_teaching_info;

std::vector<DetailDetectionStruct> g_detection_merge_result;
std::vector<DetailDetectionStruct> g_object_info_merge_result;

std::vector<WrapperBarcodeDataset> g_wrapper_barcode_data;

std::vector<CurrentTextInformation> g_text_inference_result;
std::vector<CurrentBarcodeInformation> g_barcode_reading_result;

std::vector<NepesInterface::ResultDataset> g_output_result_data;

std::vector<MESLabelInformation> g_mes_barcode_information;
std::vector<MESLabelInformation> g_mes_text_information;
std::vector<MESLabelInformation> g_mes_object_information;

std::vector<std::string> g_vision_interface_message;

std::vector<std::string> g_ai_result_type;

NepesInterface::ReelPosition g_reel_pos;
NepesInterface::ModelType g_model_type;

WrapperMESResponseSet g_mes_wrapper_response_set;
WrapperModelInformation g_wrapper_model_information;

std::string g_mes_if_base_url = "http://mmes.nepes.co.kr/MESIF/ASFC?";
std::string g_ai_if_base_url = "http://mmes.nepes.co.kr/AIIF/PI?";
std::string g_lot_number = "";

//2021-03-16 JHLEE
//짜증..Comtest용 변수
std::string g_before_lot_number = "";

PyObject* g_ai_option = nullptr;
PyObject* g_detection_model = nullptr;
PyObject* g_classification_model = nullptr;
PyThreadState* thread_state = nullptr;
PyGILState_STATE py_gil;

libxl::Book* g_book = nullptr;

bool g_loaded_ai_model = false;
bool g_already_start_ai_module = false;

bool g_reel_pass = true;
bool g_lot_pass = true;

int g_reel_total_count = 1;
int g_lot_total_count = 1;

clock_t start_total_time;
double total_tact_time = 0.;

std::wstring GetMESCommonKey(MESCommonKey mes_info)
{
	int index = (int)mes_info;
	std::wstring mes_key_array[] = {
		U("status"),
		U("msg"),
		U("rtnValue"),
		U("records")
	};
	return mes_key_array[index];
}

std::wstring GetRecipeTypeKey(MESRecipeTypeKey mes_info)
{
	int index = (int)mes_info;
	std::wstring mes_key_array[] = {
		U("sideType"),
		U("eqpRecipe"),
		U("customer")
	};
	return mes_key_array[index];
}

std::wstring GetMESRecipeInformationKey(MESRecipeInformationKey mes_info)
{
	int index = (int)mes_info;
	std::wstring mes_key_array[] = {
		U("specId"),
		U("exception"),
		U("itemType"),
		U("topLeftY"),
		U("bottomRightX"),
		U("topLeftX"),
		U("bottomLeftX"),
		U("bottomRightY"),
		U("itemSeq"),
		U("content"),
		U("itemId"),
		U("itemName"),
		U("labelId"),
		U("topRightX"),
		U("topRightY"),
		U("roiNo"),
		U("reelNo"),
		U("labelSeq"),
		U("labelName"),
		U("bottomtY")
	};
	return mes_key_array[index];
}

std::string GetMESParameterName(MESParameterName mes_param)
{
	int index = (int)mes_param;
	std::string mes_parameter_array[] = {
		"plant",
		"transaction",
		"actionType",
		"lotNumber",
		"user",
		"result",
		"operation",
		"eqp"
	};
	return mes_parameter_array[index];
}

std::string GetMESConstParameterValue(MESConstParameterValue mes_param)
{
	int index = (int)mes_param;
	std::string mes_parameter_array[] = {
		"CCUBE",
		"CCUBEDIGITAL",
		"SPIN",
		"SPOU",
		"PI_GET_EQP_RECIPE_NAME",
		"PI_GET_PISP_DATA",
		"PI_SAVE_INSP_RESULT",
		"PIAI",
		"8850"
	};
	return mes_parameter_array[index];
}

std::string GetAIResultType(int index)
{
	std::string ai_result_array[] = {
		"PASS",
		"FAIL:READING",
		"FAIL:MATCHING"
	};
	return ai_result_array[index];
}

//2020-02-17 JHLEE
//Prototype으로 모든 에러메시지를 nepes에서 관리하도록 구현해봄
//추후 횡전개 때 모든 에러 메시지를 이렇게 관리 해보자..
std::string GetVisionInterfaceMessage(int index)
{
	std::string message_array[] = {
		"작업 성공",
		"바코드 정보 구조체 배열의 크기가 잘못 되었습니다.",
		"바코드 정보 구조체 배열의 포인터가 null입니다.",
		"ROI 정보 구조체 배열의 크기가 잘못 되었습니다.",
		"ROI 정보 구조체 배열의 포인터가 null입니다.",
		"잘못된 ROI 이름입니다. 미니 라벨의 ROI 이름은 항상 1이 되어야 합니다.",
		"미니 라벨 ROI 영역에 라벨이 초과 부착 되었습니다.",
		"미니 라벨 ROI 영역에 라벨이 존재하지 않습니다.",
		"현재 릴에 라벨이 초과 부착 되었습니다. 다시 확인 하세요.",
		"현재 릴에 부착되지 않은 라벨이 있습니다. 다시 확인 하세요."
	};
	return message_array[index];
}

//Python으로 구현한 Function을 호출한다.
PyObject* PythonFunctionCall(std::string function_name, NepesInterface::AIInterfaceResultCode& return_code, PyObject* input_parameter)
{
	return_code = NepesInterface::AIInterfaceResultCode::NEPES_SUCCESS;

	PyObject* file_name = nullptr;
	PyObject* module = nullptr;
	PyObject* python_function = nullptr;
	PyObject* output_result = nullptr;

	do
	{
		file_name = PyUnicode_FromString("npsPyModule");
		if (!file_name) {
			return_code = NepesInterface::AIInterfaceResultCode::NEPES_ERROR_FAILED_TO_FIND_PYTHON_FILE;
			break;
		}

		module = PyImport_Import(file_name);
		if (!module) {
			return_code = NepesInterface::AIInterfaceResultCode::NEPES_ERROR_FAILED_TO_FIND_PYTHON_FILE;
			break;
		}
		python_function = PyObject_GetAttrString(module, function_name.c_str());
		if (!python_function) {
			return_code = NepesInterface::AIInterfaceResultCode::NEPES_ERROR_FAILED_TO_FIND_PYTHON_FUNCTION;
			break;
		}
		if (!PyCallable_Check(python_function)) {
			return_code = NepesInterface::AIInterfaceResultCode::NEPES_ERROR_FAILED_TO_CALLABLE_PYTHON_FUNCTION;
			break;
		}
		if (!input_parameter) {
			output_result = PyObject_CallFunction(python_function, nullptr);
		}
		else {
			output_result = PyObject_CallFunction(python_function, "O", input_parameter);
		}

	} while (false);

	if (file_name) {
		Py_DECREF(file_name);
	}
	
	if (module) {
		Py_DECREF(module);
	}
	
	if (python_function) {
		Py_DECREF(python_function);
	}

	return output_result;
}

//Python 이중 리스트를 C++ 벡터로 변환
NepesInterface::AIInterfaceResultCode ConvertDoubleListToVector(PyObject* double_list, std::vector<NepesInterface::RectanglePosition>& detection_result)
{
	NepesInterface::AIInterfaceResultCode return_code = NepesInterface::AIInterfaceResultCode::NEPES_SUCCESS;

	bool not_list = false;
	bool result_null = false;

	for (Py_ssize_t i = 0; i < PyList_Size(double_list); ++i) {

		PyObject* list = PyList_GetItem(double_list, i);

		if (!list) {
			result_null = true;
			break;
		}

		if (!PyList_Check(list)) {
			not_list = true;
			break;
		}

		NepesInterface::RectanglePosition detection_position;

		PyObject* position = PyList_GetItem(list, 0);
		detection_position.left_top.x = PyLong_AsLong(position);

		position = PyList_GetItem(list, 1);
		detection_position.left_top.y = PyLong_AsLong(position);

		position = PyList_GetItem(list, 2);
		detection_position.right_top.x = PyLong_AsLong(position);

		position = PyList_GetItem(list, 3);
		detection_position.right_top.y = PyLong_AsLong(position);

		position = PyList_GetItem(list, 4);
		detection_position.right_bottom.x = PyLong_AsLong(position);

		position = PyList_GetItem(list, 5);
		detection_position.right_bottom.y = PyLong_AsLong(position);

		position = PyList_GetItem(list, 6);
		detection_position.left_bottom.x = PyLong_AsLong(position);

		position = PyList_GetItem(list, 7);
		detection_position.left_bottom.y = PyLong_AsLong(position);

		detection_result.push_back(detection_position);
	}

	if (result_null) {
		return_code = NepesInterface::AIInterfaceResultCode::NEPES_ERROR_AI_DETECTION_RESULT_NULL;
	}

	if (not_list) {
		return_code = NepesInterface::AIInterfaceResultCode::NEPES_ERROR_AI_DETECTION_RESULT_TYPE_NOT_LIST;
	}

	return return_code;
}

//C++ 벡터를 Python 이중 리스트로 변환
PyObject* ConvertVectorToDoubleList(std::vector<CurrentTextInformation>& detection_result, NepesInterface::AIInterfaceResultCode& return_code)
{
	return_code = NepesInterface::AIInterfaceResultCode::NEPES_SUCCESS;

	PyObject* double_list = nullptr;

	bool result_null = false;
	bool not_list = false;

	do
	{
		double_list = PyList_New(detection_result.size());

		if (!double_list) {
			return_code = NepesInterface::AIInterfaceResultCode::NEPES_ERROR_FAILED_TO_CREATE_AI_INPUT_LIST;
			break;
		}

		for (int i = 0; i < (int)detection_result.size(); ++i) {
			PyObject* list = PyList_New(8);

			if (!list) {
				result_null = true;
				break;
			}

			PyObject* value = PyLong_FromLong(detection_result[i].bounding_boxes.left_top.x);
			if (value) {
				PyList_SetItem(list, 0, value);
			}

			value = PyLong_FromLong(detection_result[i].bounding_boxes.left_top.y);
			if (value) {
				PyList_SetItem(list, 1, value);
			}

			value = PyLong_FromLong(detection_result[i].bounding_boxes.right_top.x);
			if (value) {
				PyList_SetItem(list, 2, value);
			}

			value = PyLong_FromLong(detection_result[i].bounding_boxes.right_top.y);
			if (value) {
				PyList_SetItem(list, 3, value);
			}

			value = PyLong_FromLong(detection_result[i].bounding_boxes.right_bottom.x);
			if (value) {
				PyList_SetItem(list, 4, value);
			}

			value = PyLong_FromLong(detection_result[i].bounding_boxes.right_bottom.y);
			if (value) {
				PyList_SetItem(list, 5, value);
			}

			value = PyLong_FromLong(detection_result[i].bounding_boxes.left_bottom.x);
			if (value) {
				PyList_SetItem(list, 6, value);
			}

			value = PyLong_FromLong(detection_result[i].bounding_boxes.left_bottom.y);
			if (value) {
				PyList_SetItem(list, 7, value);
			}

			PyList_SetItem(double_list, i, list);

		}

		if (!PyList_Check(double_list)) {
			not_list = true;
			break;
		}

	} while (false);

	if (result_null) {
		return_code = NepesInterface::AIInterfaceResultCode::NEPES_ERROR_AI_DETECTION_RESULT_NULL;
	}

	if (not_list) {
		return_code = NepesInterface::AIInterfaceResultCode::NEPES_ERROR_AI_DETECTION_RESULT_TYPE_NOT_LIST;
	}

	return double_list;
}

//C++ 벡터를 Python 이중 리스트로 변환
PyObject* ConvertVectorToDoubleListInnerTuple(std::vector<DetailDetectionStruct>& detection_result, NepesInterface::AIInterfaceResultCode& return_code)
{
	return_code = NepesInterface::AIInterfaceResultCode::NEPES_SUCCESS;

	PyObject* double_list = nullptr;

	bool result_null = false;
	bool not_list = false;
	bool not_tuple = false;

	do
	{
		double_list = PyList_New(detection_result.size());

		if (!double_list) {
			return_code = NepesInterface::AIInterfaceResultCode::NEPES_ERROR_FAILED_TO_CREATE_AI_INPUT_LIST;
			break;
		}

		for (int j = 0; j < (int)detection_result.size(); ++j) {

			PyObject* list = PyList_New(detection_result[j].rct_vector.size());

			if (!list) {
				result_null = true;
				break;
			}

			for (int i = 0; i < (int)detection_result[j].rct_vector.size(); ++i) {

				PyObject* tuple_data = PyTuple_New(8);

				if (!tuple_data) {
					result_null = true;
					break;
				}
				if (!PyTuple_Check(tuple_data)) {
					not_tuple = true;
					break;
				}


				PyObject* value = PyLong_FromLong(detection_result[j].rct_vector[i].bounding_boxes.left_top.x);
				if (value) {
					PyTuple_SetItem(tuple_data, 0, value);
				}

				value = PyLong_FromLong(detection_result[j].rct_vector[i].bounding_boxes.left_top.y);
				if (value) {
					PyTuple_SetItem(tuple_data, 1, value);
				}

				value = PyLong_FromLong(detection_result[j].rct_vector[i].bounding_boxes.right_top.x);
				if (value) {
					PyTuple_SetItem(tuple_data, 2, value);
				}

				value = PyLong_FromLong(detection_result[j].rct_vector[i].bounding_boxes.right_top.y);
				if (value) {
					PyTuple_SetItem(tuple_data, 3, value);
				}

				value = PyLong_FromLong(detection_result[j].rct_vector[i].bounding_boxes.right_bottom.x);
				if (value) {
					PyTuple_SetItem(tuple_data, 4, value);
				}

				value = PyLong_FromLong(detection_result[j].rct_vector[i].bounding_boxes.right_bottom.y);
				if (value) {
					PyTuple_SetItem(tuple_data, 5, value);
				}

				value = PyLong_FromLong(detection_result[j].rct_vector[i].bounding_boxes.left_bottom.x);
				if (value) {
					PyTuple_SetItem(tuple_data, 6, value);
				}

				value = PyLong_FromLong(detection_result[j].rct_vector[i].bounding_boxes.left_bottom.y);
				if (value) {
					PyTuple_SetItem(tuple_data, 7, value);
				}

				PyList_SetItem(list, i, tuple_data);
			}

			PyList_SetItem(double_list, j, list);
		}

		if (!PyList_Check(double_list)) {
			not_list = true;
			break;
		}

	} while (false);

	if (result_null) {
		return_code = NepesInterface::AIInterfaceResultCode::NEPES_ERROR_AI_DETECTION_RESULT_NULL;
	}

	if (not_list) {
		return_code = NepesInterface::AIInterfaceResultCode::NEPES_ERROR_AI_DETECTION_RESULT_TYPE_NOT_LIST;
	}

	if (not_tuple) {
		return_code = NepesInterface::AIInterfaceResultCode::NEPES_ERROR_AI_DETECTION_RESULT_TYPE_NOT_TUPLE;
	}

	return double_list;
}

//C++ 벡터를 Python 튜플을 원소로하는 리스트로 변환
PyObject* ConvertVectorToListInnerTuple(std::vector<DetailDetectionStruct>& detection_result, NepesInterface::AIInterfaceResultCode& return_code)
{
	return_code = NepesInterface::AIInterfaceResultCode::NEPES_SUCCESS;

	PyObject* double_list = nullptr;

	bool result_null = false;
	bool not_list = false;
	bool not_tuple = false;

	do
	{
		double_list = PyList_New(detection_result.size());

		if (!double_list) {
			return_code = NepesInterface::AIInterfaceResultCode::NEPES_ERROR_FAILED_TO_CREATE_AI_INPUT_LIST;
			break;
		}

		for (int i = 0; i < (int)detection_result.size(); ++i) {

			PyObject* tuple_data = PyTuple_New(2);
			if (!tuple_data) {
				result_null = true;
				break;
			}
			if (!PyTuple_Check(tuple_data)) {
				not_tuple = true;
				break;
			}

			PyObject* list = PyList_New(8);
			if (!list) {
				result_null = true;
				break;
			}
			if (!PyList_Check(list)) {
				not_list = true;
				break;
			}

			//주의 : Py_DECREF 잘못 남용하면 주소 잃어버려서 null 참조 될 수도 있음
			//확인된 건 Tuple에 Set 하고는 하면 안됨 (Tuple 자체가 Read Only라 그럴수도 있음)

			PyObject* value = PyLong_FromLong(detection_result[i].current_text_info.bounding_boxes.left_top.x);
			if (value) {
				PyList_SetItem(list, 0, value);
			}
			//Py_DECREF(value);
			value = PyLong_FromLong(detection_result[i].current_text_info.bounding_boxes.left_top.y);
			if (value) {
				PyList_SetItem(list, 1, value);
			}
			//Py_DECREF(value);
			value = PyLong_FromLong(detection_result[i].current_text_info.bounding_boxes.right_top.x);
			if (value) {
				PyList_SetItem(list, 2, value);
			}
			//Py_DECREF(value);
			value = PyLong_FromLong(detection_result[i].current_text_info.bounding_boxes.right_top.y);
			if (value) {
				PyList_SetItem(list, 3, value);
			}
			//Py_DECREF(value);
			value = PyLong_FromLong(detection_result[i].current_text_info.bounding_boxes.right_bottom.x);
			if (value) {
				PyList_SetItem(list, 4, value);
			}
			//Py_DECREF(value);
			value = PyLong_FromLong(detection_result[i].current_text_info.bounding_boxes.right_bottom.y);
			if (value) {
				PyList_SetItem(list, 5, value);
			}
			//Py_DECREF(value);
			value = PyLong_FromLong(detection_result[i].current_text_info.bounding_boxes.left_bottom.x);
			if (value) {
				PyList_SetItem(list, 6, value);
			}
			//Py_DECREF(value);
			value = PyLong_FromLong(detection_result[i].current_text_info.bounding_boxes.left_bottom.y);
			if (value) {
				PyList_SetItem(list, 7, value);
			}
			//Py_DECREF(value);
			PyObject* number = PyLong_FromLong(detection_result[i].current_text_info.object_id);

			PyTuple_SetItem(tuple_data, 0, number);
			PyTuple_SetItem(tuple_data, 1, list);

			PyList_SetItem(double_list, i, tuple_data);

			//Py_DECREF(number);
			//Py_DECREF(list);
			//Py_DECREF(tuple_data);
		}

		if (!PyList_Check(double_list)) {
			not_list = true;
			break;
		}

	} while (false);

	if (result_null) {
		return_code = NepesInterface::AIInterfaceResultCode::NEPES_ERROR_AI_DETECTION_RESULT_NULL;
	}

	if (not_list) {
		return_code = NepesInterface::AIInterfaceResultCode::NEPES_ERROR_AI_DETECTION_RESULT_TYPE_NOT_LIST;
	}

	if (not_tuple) {
		return_code = NepesInterface::AIInterfaceResultCode::NEPES_ERROR_AI_DETECTION_RESULT_TYPE_NOT_TUPLE;
	}

	return double_list;
}

//Python 튜플을 원소로하는 리스트를 C++ 벡터로 변환
NepesInterface::AIInterfaceResultCode ConvertListInnerTupleToVector(PyObject* double_list, std::vector<CurrentTextInformation>& inference_result)
{
	NepesInterface::AIInterfaceResultCode return_code = NepesInterface::AIInterfaceResultCode::NEPES_SUCCESS;

	bool result_null = false;
	bool not_tuple = false;

	do
	{
		if (!double_list) {

		}
		for (Py_ssize_t i = 0; i < PyList_Size(double_list); ++i) {

			PyObject* tuple_data = PyList_GetItem(double_list, i);
			if (!tuple_data) {
				result_null = true;
				break;
			}
			if (!PyTuple_Check(tuple_data)) {
				not_tuple = true;
				break;
			}

			CurrentTextInformation text_info;

			PyObject* number = PyTuple_GetItem(tuple_data, 0);
			text_info.object_id = PyLong_AsLong(number);

			PyObject* string_data = PyTuple_GetItem(tuple_data, 1);
			text_info.text_data = PyUnicode_AsUTF8(string_data);

			inference_result.push_back(text_info);
		}

	} while (false);

	if (result_null) {
		return_code = NepesInterface::AIInterfaceResultCode::NEPES_ERROR_AI_CLASSIFICATION_RESULT_NULL;
	}

	if (not_tuple) {
		return_code = NepesInterface::AIInterfaceResultCode::NEPES_ERROR_AI_CLASSIFICATION_RESULT_TYPE_NOT_TUPLE;
	}

	return return_code;
}

//현재 라벨의 텍스트 정보를 MES 기준 정보들로 부터 찾는다.
bool GetCurrentLabelTextInformation(_In_ NepesInterface::ReelPosition reel_pos, _In_ int reel_no, _In_ int current_label_index)
{
	int label_seq = current_label_index + 1;
	int reel_number = reel_no + 1;

	if (label_seq < 0) {
		return false;
	}

	if (g_mes_text_information.size() <= 0) {
		return false;
	}

	switch (g_model_type)
	{
		//단면 래시피
		case NepesInterface::ModelType::TYPE_ONE_SIDE:
		{
			for (int i = 0; i < g_mes_text_information.size(); ++i) {

				int roi_number = std::stoi(g_mes_text_information[i].roi_name);

				//MES 기준 정보들에서 현재 라벨에 대한 정보를 찾는다.
				//기준 1 : ROI Name (Vision에서 설정한 ROI Name과 MES에 저장한 ROI Name은 반드시 같게 셋업하도록 약속됨)
				//기준 2 : Reel Number
				if (roi_number == label_seq && g_mes_text_information[i].reel_number == reel_number) {

					CurrentTextInformation current_text_info;

					current_text_info.object_id = i;
					current_text_info.bounding_boxes.left_top.x = g_mes_text_information[i].teaching_position.left_top.x;
					current_text_info.bounding_boxes.left_top.y = g_mes_text_information[i].teaching_position.left_top.y;
					current_text_info.bounding_boxes.right_top.x = g_mes_text_information[i].teaching_position.right_top.x;
					current_text_info.bounding_boxes.right_top.y = g_mes_text_information[i].teaching_position.right_top.y;
					current_text_info.bounding_boxes.right_bottom.x = g_mes_text_information[i].teaching_position.right_bottom.x;
					current_text_info.bounding_boxes.right_bottom.y = g_mes_text_information[i].teaching_position.right_bottom.y;
					current_text_info.bounding_boxes.left_bottom.x = g_mes_text_information[i].teaching_position.left_bottom.x;
					current_text_info.bounding_boxes.left_bottom.y = g_mes_text_information[i].teaching_position.left_bottom.y;
					current_text_info.center_position.x = (current_text_info.bounding_boxes.left_top.x + current_text_info.bounding_boxes.right_top.x) / 2;
					current_text_info.center_position.y = (current_text_info.bounding_boxes.right_top.y + current_text_info.bounding_boxes.right_bottom.y) / 2;
					current_text_info.text_data = g_mes_text_information[i].content;
					current_text_info.comparison = false;
					current_text_info.exception = g_mes_text_information[i].exception;
					current_text_info.reel_no = reel_no;
					current_text_info.label_no = current_label_index;
					current_text_info.image_name = g_image_path;
					current_text_info.item_name = g_mes_text_information[i].item_name;

					g_current_text_info.push_back(current_text_info);
				}
			}
		}
		break;

		//양면 래시피
		case NepesInterface::ModelType::TYPE_DOUBLE_SIDE:
		{
			if (reel_pos == NepesInterface::ReelPosition::POS_FRONT) {
				for (int i = 0; i < g_mes_text_information.size(); ++i) {

					std::string label_seq_text = std::to_string(label_seq);

					//양면 래시피의 경우 앞면은 ROI Name_F로 약속함
					label_seq_text = label_seq_text + "_F";
	
					//MES 기준 정보들에서 현재 라벨에 대한 정보를 찾는다.
					//기준 1 : ROI Name (Vision에서 설정한 ROI Name과 MES에 저장한 ROI Name은 반드시 같게 셋업하도록 약속됨)
					//기준 2 : Reel Number
					if (g_mes_text_information[i].roi_name == label_seq_text && g_mes_text_information[i].reel_number == reel_number) {

						CurrentTextInformation current_text_info;

						current_text_info.object_id = i;
						current_text_info.bounding_boxes.left_top.x = g_mes_text_information[i].teaching_position.left_top.x;
						current_text_info.bounding_boxes.left_top.y = g_mes_text_information[i].teaching_position.left_top.y;
						current_text_info.bounding_boxes.right_top.x = g_mes_text_information[i].teaching_position.right_top.x;
						current_text_info.bounding_boxes.right_top.y = g_mes_text_information[i].teaching_position.right_top.y;
						current_text_info.bounding_boxes.right_bottom.x = g_mes_text_information[i].teaching_position.right_bottom.x;
						current_text_info.bounding_boxes.right_bottom.y = g_mes_text_information[i].teaching_position.right_bottom.y;
						current_text_info.bounding_boxes.left_bottom.x = g_mes_text_information[i].teaching_position.left_bottom.x;
						current_text_info.bounding_boxes.left_bottom.y = g_mes_text_information[i].teaching_position.left_bottom.y;
						current_text_info.center_position.x = (current_text_info.bounding_boxes.left_top.x + current_text_info.bounding_boxes.right_top.x) / 2;
						current_text_info.center_position.y = (current_text_info.bounding_boxes.right_top.y + current_text_info.bounding_boxes.right_bottom.y) / 2;
						current_text_info.text_data = g_mes_text_information[i].content;
						current_text_info.comparison = false;
						current_text_info.exception = g_mes_text_information[i].exception;
						current_text_info.reel_no = reel_no;
						current_text_info.label_no = current_label_index;
						current_text_info.image_name = g_image_path;
						current_text_info.item_name = g_mes_text_information[i].item_name;

						g_current_text_info.push_back(current_text_info);
					}
				}
			}
			else if (reel_pos == NepesInterface::ReelPosition::POS_BACK) {
				for (int i = 0; i < g_mes_text_information.size(); ++i) {

					std::string label_seq_text = std::to_string(label_seq);

					//양면 래시피의 경우 뒷면은 ROI Name_B로 약속함
					label_seq_text = label_seq_text + "_B";

					//MES 기준 정보들에서 현재 라벨에 대한 정보를 찾는다.
					//기준 1 : ROI Name (Vision에서 설정한 ROI Name과 MES에 저장한 ROI Name은 반드시 같게 셋업하도록 약속됨)
					//기준 2 : Reel Number
					if (g_mes_text_information[i].roi_name == label_seq_text && g_mes_text_information[i].reel_number == reel_number) {

						CurrentTextInformation current_text_info;

						current_text_info.object_id = i;
						current_text_info.bounding_boxes.left_top.x = g_mes_text_information[i].teaching_position.left_top.x;
						current_text_info.bounding_boxes.left_top.y = g_mes_text_information[i].teaching_position.left_top.y;
						current_text_info.bounding_boxes.right_top.x = g_mes_text_information[i].teaching_position.right_top.x;
						current_text_info.bounding_boxes.right_top.y = g_mes_text_information[i].teaching_position.right_top.y;
						current_text_info.bounding_boxes.right_bottom.x = g_mes_text_information[i].teaching_position.right_bottom.x;
						current_text_info.bounding_boxes.right_bottom.y = g_mes_text_information[i].teaching_position.right_bottom.y;
						current_text_info.bounding_boxes.left_bottom.x = g_mes_text_information[i].teaching_position.left_bottom.x;
						current_text_info.bounding_boxes.left_bottom.y = g_mes_text_information[i].teaching_position.left_bottom.y;
						current_text_info.center_position.x = (current_text_info.bounding_boxes.left_top.x + current_text_info.bounding_boxes.right_top.x) / 2;
						current_text_info.center_position.y = (current_text_info.bounding_boxes.right_top.y + current_text_info.bounding_boxes.right_bottom.y) / 2;
						current_text_info.text_data = g_mes_text_information[i].content;
						current_text_info.comparison = false;
						current_text_info.exception = g_mes_text_information[i].exception;
						current_text_info.reel_no = reel_no;
						current_text_info.label_no = current_label_index;
						current_text_info.image_name = g_image_path;
						current_text_info.item_name = g_mes_text_information[i].item_name;

						g_current_text_info.push_back(current_text_info);
					}
				}
			}
		
		}
		break;

		default:
		break;
	}

	return true;
}

//현재 라벨의 바코드 정보를 MES 기준 정보들로 부터 찾는다.
bool GetCurrentLabelBarcodeInformation(_In_ NepesInterface::ReelPosition reel_pos, _In_ int reel_no, _In_ int current_label_index)
{
	int label_seq = current_label_index + 1;
	int reel_number = reel_no + 1;

	if (label_seq < 0) {
		return false;
	}

	if (g_mes_barcode_information.size() <= 0) {
		return false;
	}

	switch (g_model_type)
	{

	//단면 래시피
	case NepesInterface::ModelType::TYPE_ONE_SIDE:
	{
		for (int i = 0; i < g_mes_barcode_information.size(); ++i) {

			int roi_number = std::stoi(g_mes_barcode_information[i].roi_name);

			//MES 기준 정보들에서 현재 라벨에 대한 정보를 찾는다.
			//기준 1 : ROI Name (Vision에서 설정한 ROI Name과 MES에 저장한 ROI Name은 반드시 같게 셋업하도록 약속됨)
			//기준 2 : Reel Number
			if (roi_number == label_seq && g_mes_barcode_information[i].reel_number == reel_number) {

				CurrentBarcodeInformation current_barcode_info;

				current_barcode_info.object_id = i;
				current_barcode_info.bounding_boxes.left_top.x = g_mes_barcode_information[i].teaching_position.left_top.x;
				current_barcode_info.bounding_boxes.left_top.y = g_mes_barcode_information[i].teaching_position.left_top.y;
				current_barcode_info.bounding_boxes.right_top.x = g_mes_barcode_information[i].teaching_position.right_top.x;
				current_barcode_info.bounding_boxes.right_top.y = g_mes_barcode_information[i].teaching_position.right_top.y;
				current_barcode_info.bounding_boxes.right_bottom.x = g_mes_barcode_information[i].teaching_position.right_bottom.x;
				current_barcode_info.bounding_boxes.right_bottom.y = g_mes_barcode_information[i].teaching_position.right_bottom.y;
				current_barcode_info.bounding_boxes.left_bottom.x = g_mes_barcode_information[i].teaching_position.left_bottom.x;
				current_barcode_info.bounding_boxes.left_bottom.y = g_mes_barcode_information[i].teaching_position.left_bottom.y;
				current_barcode_info.center_position.x = (current_barcode_info.bounding_boxes.left_top.x + current_barcode_info.bounding_boxes.right_top.x) / 2;
				current_barcode_info.center_position.y = (current_barcode_info.bounding_boxes.right_top.y + current_barcode_info.bounding_boxes.right_bottom.y) / 2;
				current_barcode_info.barcode_data = g_mes_barcode_information[i].content;
				current_barcode_info.comparison = false;
				current_barcode_info.exception = g_mes_barcode_information[i].exception;
				current_barcode_info.reel_no = reel_no;
				current_barcode_info.label_no = current_label_index;
				current_barcode_info.image_name = g_image_path;

				g_barcode_teaching_info.push_back(current_barcode_info);
			}
		}
	}
	break;

	//양면 래시피
	case NepesInterface::ModelType::TYPE_DOUBLE_SIDE:
	{
		if (reel_pos == NepesInterface::ReelPosition::POS_FRONT) {
			for (int i = 0; i < g_mes_barcode_information.size(); ++i) {


				std::string label_seq_text = std::to_string(label_seq);

				//양면 래시피의 경우 앞면은 ROI Name_F로 약속함
				label_seq_text = label_seq_text + "_F";

				//MES 기준 정보들에서 현재 라벨에 대한 정보를 찾는다.
				//기준 1 : ROI Name (Vision에서 설정한 ROI Name과 MES에 저장한 ROI Name은 반드시 같게 셋업하도록 약속됨)
				//기준 2 : Reel Number
				if (g_mes_barcode_information[i].roi_name == label_seq_text && g_mes_barcode_information[i].reel_number == reel_number) {

					CurrentBarcodeInformation current_barcode_info;

					current_barcode_info.object_id = i;
					current_barcode_info.bounding_boxes.left_top.x = g_mes_barcode_information[i].teaching_position.left_top.x;
					current_barcode_info.bounding_boxes.left_top.y = g_mes_barcode_information[i].teaching_position.left_top.y;
					current_barcode_info.bounding_boxes.right_top.x = g_mes_barcode_information[i].teaching_position.right_top.x;
					current_barcode_info.bounding_boxes.right_top.y = g_mes_barcode_information[i].teaching_position.right_top.y;
					current_barcode_info.bounding_boxes.right_bottom.x = g_mes_barcode_information[i].teaching_position.right_bottom.x;
					current_barcode_info.bounding_boxes.right_bottom.y = g_mes_barcode_information[i].teaching_position.right_bottom.y;
					current_barcode_info.bounding_boxes.left_bottom.x = g_mes_barcode_information[i].teaching_position.left_bottom.x;
					current_barcode_info.bounding_boxes.left_bottom.y = g_mes_barcode_information[i].teaching_position.left_bottom.y;
					current_barcode_info.center_position.x = (current_barcode_info.bounding_boxes.left_top.x + current_barcode_info.bounding_boxes.right_top.x) / 2;
					current_barcode_info.center_position.y = (current_barcode_info.bounding_boxes.right_top.y + current_barcode_info.bounding_boxes.right_bottom.y) / 2;
					current_barcode_info.barcode_data = g_mes_barcode_information[i].content;
					current_barcode_info.comparison = false;
					current_barcode_info.exception = g_mes_barcode_information[i].exception;
					current_barcode_info.reel_no = reel_no;
					current_barcode_info.label_no = current_label_index;
					current_barcode_info.image_name = g_image_path;

					g_barcode_teaching_info.push_back(current_barcode_info);
				}
			}
		}
		else if (reel_pos == NepesInterface::ReelPosition::POS_BACK) {
			for (int i = 0; i < g_mes_barcode_information.size(); ++i) {

				std::string label_seq_text = std::to_string(label_seq);

				//양면 래시피의 경우 뒷면은 ROI Name_B로 약속함
				label_seq_text = label_seq_text + "_B";

				//MES 기준 정보들에서 현재 라벨에 대한 정보를 찾는다.
				//기준 1 : ROI Name (Vision에서 설정한 ROI Name과 MES에 저장한 ROI Name은 반드시 같게 셋업하도록 약속됨)
				//기준 2 : Reel Number
				if (g_mes_barcode_information[i].roi_name == label_seq_text && g_mes_barcode_information[i].reel_number == reel_number) {

					CurrentBarcodeInformation current_barcode_info;

					current_barcode_info.object_id = i;
					current_barcode_info.bounding_boxes.left_top.x = g_mes_barcode_information[i].teaching_position.left_top.x;
					current_barcode_info.bounding_boxes.left_top.y = g_mes_barcode_information[i].teaching_position.left_top.y;
					current_barcode_info.bounding_boxes.right_top.x = g_mes_barcode_information[i].teaching_position.right_top.x;
					current_barcode_info.bounding_boxes.right_top.y = g_mes_barcode_information[i].teaching_position.right_top.y;
					current_barcode_info.bounding_boxes.right_bottom.x = g_mes_barcode_information[i].teaching_position.right_bottom.x;
					current_barcode_info.bounding_boxes.right_bottom.y = g_mes_barcode_information[i].teaching_position.right_bottom.y;
					current_barcode_info.bounding_boxes.left_bottom.x = g_mes_barcode_information[i].teaching_position.left_bottom.x;
					current_barcode_info.bounding_boxes.left_bottom.y = g_mes_barcode_information[i].teaching_position.left_bottom.y;
					current_barcode_info.center_position.x = (current_barcode_info.bounding_boxes.left_top.x + current_barcode_info.bounding_boxes.right_top.x) / 2;
					current_barcode_info.center_position.y = (current_barcode_info.bounding_boxes.right_top.y + current_barcode_info.bounding_boxes.right_bottom.y) / 2;
					current_barcode_info.barcode_data = g_mes_barcode_information[i].content;
					current_barcode_info.comparison = false;
					current_barcode_info.exception = g_mes_barcode_information[i].exception;
					current_barcode_info.reel_no = reel_no;
					current_barcode_info.label_no = current_label_index;
					current_barcode_info.image_name = g_image_path;

					g_barcode_teaching_info.push_back(current_barcode_info);
				}
			}
		}

	}
	break;

	default:
		break;
	}

	return true;
}

//현재 라벨의 Object 정보를 MES 기준 정보들로 부터 찾는다.
//Object : 글자를 Detection 할 순 있으나, Inference 하기는 힘든 글자들
//         라벨 발행 시 정보가 아닌 이미지 형태로 프린트되는 것들
bool GetCurrentLabelObjectInformation(_In_ NepesInterface::ReelPosition reel_pos, _In_ int reel_no, _In_ int current_label_index)
{
	int label_seq = current_label_index + 1;
	int reel_number = reel_no + 1;

	if (label_seq < 0) {
		return false;
	}

	if (g_mes_object_information.size() <= 0) {
		return false;
	}

	switch (g_model_type)
	{
	case NepesInterface::ModelType::TYPE_ONE_SIDE:
	{
		//2020-11-10 JHLEE 추가
		for (int i = 0; i < g_mes_object_information.size(); ++i) {

			int roi_number = std::stoi(g_mes_object_information[i].roi_name);

			//MES 기준 정보들에서 현재 라벨에 대한 정보를 찾는다.
			//기준 1 : ROI Name (Vision에서 설정한 ROI Name과 MES에 저장한 ROI Name은 반드시 같게 셋업하도록 약속됨)
			//기준 2 : Reel Number
			if (roi_number == label_seq && g_mes_object_information[i].reel_number == reel_number) {

				CurrentTextInformation current_text_info;

				current_text_info.object_id = i;
				current_text_info.bounding_boxes.left_top.x = g_mes_object_information[i].teaching_position.left_top.x;
				current_text_info.bounding_boxes.left_top.y = g_mes_object_information[i].teaching_position.left_top.y;
				current_text_info.bounding_boxes.right_top.x = g_mes_object_information[i].teaching_position.right_top.x;
				current_text_info.bounding_boxes.right_top.y = g_mes_object_information[i].teaching_position.right_top.y;
				current_text_info.bounding_boxes.right_bottom.x = g_mes_object_information[i].teaching_position.right_bottom.x;
				current_text_info.bounding_boxes.right_bottom.y = g_mes_object_information[i].teaching_position.right_bottom.y;
				current_text_info.bounding_boxes.left_bottom.x = g_mes_object_information[i].teaching_position.left_bottom.x;
				current_text_info.bounding_boxes.left_bottom.y = g_mes_object_information[i].teaching_position.left_bottom.y;
				current_text_info.center_position.x = (current_text_info.bounding_boxes.left_top.x + current_text_info.bounding_boxes.right_top.x) / 2;
				current_text_info.center_position.y = (current_text_info.bounding_boxes.right_top.y + current_text_info.bounding_boxes.right_bottom.y) / 2;
				current_text_info.text_data = g_mes_object_information[i].content;
				current_text_info.comparison = false;
				current_text_info.exception = g_mes_object_information[i].exception;
				current_text_info.reel_no = reel_no;
				current_text_info.label_no = current_label_index;
				current_text_info.image_name = g_image_path;

				g_object_teaching_info.push_back(current_text_info);
			}
		}
	}
	break;

	case NepesInterface::ModelType::TYPE_DOUBLE_SIDE:
	{
		if (reel_pos == NepesInterface::ReelPosition::POS_FRONT) {
			for (int i = 0; i < g_mes_object_information.size(); ++i) {

				std::string label_seq_text = std::to_string(label_seq);

				//양면 래시피의 경우 앞면은 ROI Name_F로 약속함
				label_seq_text = label_seq_text + "_F";

				//MES 기준 정보들에서 현재 라벨에 대한 정보를 찾는다.
				//기준 1 : ROI Name (Vision에서 설정한 ROI Name과 MES에 저장한 ROI Name은 반드시 같게 셋업하도록 약속됨)
				//기준 2 : Reel Number
				if (g_mes_object_information[i].roi_name == label_seq_text && g_mes_object_information[i].reel_number == reel_number) {

					CurrentTextInformation current_text_info;

					current_text_info.object_id = i;
					current_text_info.bounding_boxes.left_top.x = g_mes_object_information[i].teaching_position.left_top.x;
					current_text_info.bounding_boxes.left_top.y = g_mes_object_information[i].teaching_position.left_top.y;
					current_text_info.bounding_boxes.right_top.x = g_mes_object_information[i].teaching_position.right_top.x;
					current_text_info.bounding_boxes.right_top.y = g_mes_object_information[i].teaching_position.right_top.y;
					current_text_info.bounding_boxes.right_bottom.x = g_mes_object_information[i].teaching_position.right_bottom.x;
					current_text_info.bounding_boxes.right_bottom.y = g_mes_object_information[i].teaching_position.right_bottom.y;
					current_text_info.bounding_boxes.left_bottom.x = g_mes_object_information[i].teaching_position.left_bottom.x;
					current_text_info.bounding_boxes.left_bottom.y = g_mes_object_information[i].teaching_position.left_bottom.y;
					current_text_info.center_position.x = (current_text_info.bounding_boxes.left_top.x + current_text_info.bounding_boxes.right_top.x) / 2;
					current_text_info.center_position.y = (current_text_info.bounding_boxes.right_top.y + current_text_info.bounding_boxes.right_bottom.y) / 2;
					current_text_info.text_data = g_mes_object_information[i].content;
					current_text_info.comparison = false;
					current_text_info.exception = g_mes_object_information[i].exception;
					current_text_info.reel_no = reel_no;
					current_text_info.label_no = current_label_index;
					current_text_info.image_name = g_image_path;

					g_object_teaching_info.push_back(current_text_info);
				}
			}
		}
		else if (reel_pos == NepesInterface::ReelPosition::POS_BACK) {
			for (int i = 0; i < g_mes_object_information.size(); ++i) {

				std::string label_seq_text = std::to_string(label_seq);

				//양면 래시피의 경우 뒷면은 ROI Name_B로 약속함
				label_seq_text = label_seq_text + "_B";

				//MES 기준 정보들에서 현재 라벨에 대한 정보를 찾는다.
				//기준 1 : ROI Name (Vision에서 설정한 ROI Name과 MES에 저장한 ROI Name은 반드시 같게 셋업하도록 약속됨)
				//기준 2 : Reel Number
				if (g_mes_object_information[i].roi_name == label_seq_text && g_mes_object_information[i].reel_number == reel_number) {

					CurrentTextInformation current_text_info;

					current_text_info.object_id = i;
					current_text_info.bounding_boxes.left_top.x = g_mes_object_information[i].teaching_position.left_top.x;
					current_text_info.bounding_boxes.left_top.y = g_mes_object_information[i].teaching_position.left_top.y;
					current_text_info.bounding_boxes.right_top.x = g_mes_object_information[i].teaching_position.right_top.x;
					current_text_info.bounding_boxes.right_top.y = g_mes_object_information[i].teaching_position.right_top.y;
					current_text_info.bounding_boxes.right_bottom.x = g_mes_object_information[i].teaching_position.right_bottom.x;
					current_text_info.bounding_boxes.right_bottom.y = g_mes_object_information[i].teaching_position.right_bottom.y;
					current_text_info.bounding_boxes.left_bottom.x = g_mes_object_information[i].teaching_position.left_bottom.x;
					current_text_info.bounding_boxes.left_bottom.y = g_mes_object_information[i].teaching_position.left_bottom.y;
					current_text_info.center_position.x = (current_text_info.bounding_boxes.left_top.x + current_text_info.bounding_boxes.right_top.x) / 2;
					current_text_info.center_position.y = (current_text_info.bounding_boxes.right_top.y + current_text_info.bounding_boxes.right_bottom.y) / 2;
					current_text_info.text_data = g_mes_object_information[i].content;
					current_text_info.comparison = false;
					current_text_info.exception = g_mes_object_information[i].exception;
					current_text_info.reel_no = reel_no;
					current_text_info.label_no = current_label_index;
					current_text_info.image_name = g_image_path;

					g_object_teaching_info.push_back(current_text_info);
				}
			}
		}

	}
	break;

	default:
		break;
	}

	return true;
}

// AI Detection Model에 넣기 전에 Object ID 오름차순으로 정렬
bool SortTextInformationObjectID(DetailDetectionStruct data1, DetailDetectionStruct data2)
{
	if (data1.current_text_info.object_id < data2.current_text_info.object_id) {
		return true;
	}
	else {
		return false;
	}
}

// 검사 결과를 Vision에게 전송하기 전에 Object ID 오름차순으로 정렬
bool SortResultObjectID(NepesInterface::ResultDataset data1, NepesInterface::ResultDataset data2)
{
	if (data1.object_id < data2.object_id) {
		return true;
	}
	else {
		return false;
	}
}

// AI Text Detection 결과 영역들을 X좌표 순으로 오름차순 정렬
bool SortMinXTempDetectionResult(CurrentTextInformation data1, CurrentTextInformation data2)
{
	if (data1.bounding_boxes.left_top.x < data2.bounding_boxes.left_top.x) {
		return true;
	}
	else {
		return false;
	}
}

// MES에서 티칭한 좌표를 바탕으로 AI Detection 결과들을 기준 정보 단위로 묶는다.
// 이유 : AI Dection Model이 찾은 Output들은 항상 일정한 단위로 Detection하지 못한다. (Pre-Trained Model을 사용해서..)
// ex : MES 기준정보는 LOT : ABCDE
//      AI Detection Model 결과 : (LOT) (:) (ABCDE) 이런식으로 찾기 때문...
std::vector<std::vector<CurrentTextInformation>> GetInformationFromMESTeachingBox(std::vector<NepesInterface::RectanglePosition>& detection_result, int left_top_y_diff, int left_bottom_y_diff)
{
	std::vector<std::vector<CurrentTextInformation>> same_line_text_info;

	for (int j = 0; j < g_current_text_info.size(); ++j) {

		std::vector<CurrentTextInformation> text_info;

		for (auto iter = detection_result.begin(); iter != detection_result.end();) {

			NepesInterface::RectanglePosition detection_position = *iter;
			detection_position.left_top.y += left_top_y_diff;
			detection_position.right_top.y = detection_position.left_top.y;
			detection_position.left_bottom.y += left_bottom_y_diff;
			detection_position.right_bottom.y = detection_position.left_bottom.y;

			NepesInterface::Vertex this_center;
			this_center.x = (detection_position.left_top.x + detection_position.right_top.x) / 2;
			this_center.y = (detection_position.left_top.y + detection_position.right_bottom.y) / 2;

			if (this_center.x >= g_current_text_info[j].bounding_boxes.left_top.x &&
				this_center.y >= g_current_text_info[j].bounding_boxes.left_top.y &&
				this_center.x <= g_current_text_info[j].bounding_boxes.right_bottom.x &&
				this_center.y <= g_current_text_info[j].bounding_boxes.right_bottom.y) {

				CurrentTextInformation dataset;
				dataset.center_position = this_center;

				dataset.bounding_boxes.left_top.x = iter->left_top.x;
				dataset.bounding_boxes.left_top.y = iter->left_top.y;
				dataset.bounding_boxes.right_top.x = iter->right_top.x;
				dataset.bounding_boxes.right_top.y = iter->right_top.y;
				dataset.bounding_boxes.right_bottom.x = iter->right_bottom.x;
				dataset.bounding_boxes.right_bottom.y = iter->right_bottom.y;
				dataset.bounding_boxes.left_bottom.x = iter->left_bottom.x;
				dataset.bounding_boxes.left_bottom.y = iter->left_bottom.y;

				dataset.object_id = g_current_text_info[j].object_id;
				dataset.reel_no = g_current_text_info[j].reel_no;
				dataset.label_no = g_current_text_info[j].label_no;
				dataset.image_name = g_image_path;

				text_info.push_back(dataset);
				iter = detection_result.erase(iter);
			}
			else {
				++iter;
			}
		}
		same_line_text_info.push_back(text_info);
	}

	return same_line_text_info;
}

// MES에서 티칭한 좌표를 바탕으로 MES 바코드 기준 정보와 Vision에서 읽은 바코드 결과를 매칭시킨다.
bool AlignBarcodeReadingResult(_In_ int reel_no)
{
	bool result = false;

	std::ofstream output_file("D:\\Result\\Log3.txt", std::ios::app);

	do
	{
		std::vector<WrapperBarcodeDataset> temp_data = g_wrapper_barcode_data;
		if (!temp_data.size()) {
			output_file << "g_wrapper_barcode_data size is zero." << std::endl;
			break;
		}

		//이 함수에서만 비교하기 위해 만든 임시 변수
		std::vector<CurrentBarcodeInformation> barcode_align_result = g_barcode_teaching_info;
		if (!barcode_align_result.size()) {
			output_file << "barcode_align_result size is zero." << std::endl;
			break;
		}

		std::vector<int> average_vector;

		for (int i = 0; i < g_barcode_teaching_info.size(); ++i) {

			if (barcode_align_result[i].comparison) {
				continue;
			}

			for (auto iter = temp_data.begin(); iter != temp_data.end();) {

				if (iter->center_position.x >= g_barcode_teaching_info[i].bounding_boxes.left_top.x &&
					iter->center_position.y >= g_barcode_teaching_info[i].bounding_boxes.left_top.y &&
					iter->center_position.x <= g_barcode_teaching_info[i].bounding_boxes.right_bottom.x &&
					iter->center_position.y <= g_barcode_teaching_info[i].bounding_boxes.right_bottom.y) {

					CurrentBarcodeInformation barcode_info;
					barcode_info.object_id = g_barcode_teaching_info[i].object_id;
					barcode_info.barcode_data = iter->barcode_reading_data;
					barcode_info.center_position = iter->center_position;
					barcode_info.comparison = false;
					barcode_info.reel_no = g_barcode_teaching_info[i].reel_no;
					barcode_info.label_no = g_barcode_teaching_info[i].label_no;
					barcode_info.image_name = g_image_path;

					g_barcode_reading_result.push_back(barcode_info);

					iter = temp_data.erase(iter);

					barcode_align_result[i].comparison = true;

					//추가
					//실패할 수도 있으니까...
					int teaching_center_y = (g_barcode_teaching_info[i].bounding_boxes.right_bottom.y + g_barcode_teaching_info[i].bounding_boxes.right_top.y) / 2;
					int y_difference = teaching_center_y - barcode_info.center_position.y;
					average_vector.push_back(y_difference);
				}
				else {
					++iter;
				}
			}
		}

		if (temp_data.size() <= 0) {
			output_file << "temp_data size is zero." << std::endl;
			result = true;
			break;
		}

		//2021-01-13 JHLEE
		//여길 Y좌표 차이 만큼 Shift 하도록 바꿔야함

		//만약 데이터가 1개면 -> 그냥 남은 바코드에 포함시키면 됨
		if (temp_data.size() == 1) {
			for (int i = 0; i < g_barcode_teaching_info.size(); ++i) {
				if (barcode_align_result[i].comparison) {
					continue;
				}

				CurrentBarcodeInformation barcode_info;
				barcode_info.object_id = g_barcode_teaching_info[i].object_id;
				barcode_info.barcode_data = temp_data[0].barcode_reading_data;
				barcode_info.center_position = temp_data[0].center_position;
				barcode_info.comparison = false;
				barcode_info.reel_no = g_barcode_teaching_info[i].reel_no;
				barcode_info.label_no = g_barcode_teaching_info[i].label_no;
				barcode_info.image_name = g_image_path;

				g_barcode_reading_result.push_back(barcode_info);

				barcode_align_result[i].comparison = true;
			}
		}

		//만약 데이터가 2개 이상이면
		else {

			if (!average_vector.size()) {
				output_file << "average_vector size is zero." << std::endl;
				break;
			}

			int y_difference = 0;

			for (int vector_size = 0; vector_size < average_vector.size(); ++vector_size) {
				y_difference += average_vector[vector_size];
			}

			y_difference = y_difference / (int)average_vector.size();

			for (int i = 0; i < g_barcode_teaching_info.size(); ++i) {

				if (barcode_align_result[i].comparison) {
					continue;
				}

				for (auto iter = temp_data.begin(); iter != temp_data.end();) {
					WrapperBarcodeDataset temp_dataset = *iter;
					temp_dataset.center_position.y += y_difference;

					if (iter->center_position.x >= g_barcode_teaching_info[i].bounding_boxes.left_top.x &&
						iter->center_position.y >= g_barcode_teaching_info[i].bounding_boxes.left_top.y &&
						iter->center_position.x <= g_barcode_teaching_info[i].bounding_boxes.right_bottom.x &&
						iter->center_position.y <= g_barcode_teaching_info[i].bounding_boxes.right_bottom.y) {

						CurrentBarcodeInformation barcode_info;
						barcode_info.object_id = g_barcode_teaching_info[i].object_id;
						barcode_info.barcode_data = iter->barcode_reading_data;
						barcode_info.center_position = iter->center_position;
						barcode_info.comparison = false;
						barcode_info.reel_no = g_barcode_teaching_info[i].reel_no;
						barcode_info.label_no = g_barcode_teaching_info[i].label_no;
						barcode_info.image_name = g_image_path;

						g_barcode_reading_result.push_back(barcode_info);

						iter = temp_data.erase(iter);

						barcode_align_result[i].comparison = true;
					}
					else {
						++iter;
					}

				}

			}
		}

		result = true;

	} while (false);

	output_file.close();

	return result;
}

// MES에서 티칭한 좌표를 바탕으로 Object 정보들을 하나 정보로 묶는다.
std::vector<std::vector<CurrentTextInformation>> AlignObjectReadingResult(std::vector<NepesInterface::RectanglePosition>& detection_result, int left_top_y_diff, int left_bottom_y_diff)
{
	std::vector<std::vector<CurrentTextInformation>> same_region_object_info;

	for (int j = 0; j < g_object_teaching_info.size(); ++j) {

		std::vector<CurrentTextInformation> object_info;

		for (auto iter = detection_result.begin(); iter != detection_result.end();) {

			NepesInterface::RectanglePosition detection_position = *iter;
			detection_position.left_top.y += left_top_y_diff;
			detection_position.right_top.y = detection_position.left_top.y;
			detection_position.left_bottom.y += left_bottom_y_diff;
			detection_position.right_bottom.y = detection_position.left_bottom.y;

			NepesInterface::Vertex this_center;
			this_center.x = (detection_position.left_top.x + detection_position.right_top.x) / 2;
			this_center.y = (detection_position.left_top.y + detection_position.right_bottom.y) / 2;

			//std::cout << "처음한거 X : " << this_center.x << " Y : " << this_center.y << std::endl;

			if (this_center.x >= g_object_teaching_info[j].bounding_boxes.left_top.x &&
				this_center.y >= g_object_teaching_info[j].bounding_boxes.left_top.y &&
				this_center.x <= g_object_teaching_info[j].bounding_boxes.right_bottom.x &&
				this_center.y <= g_object_teaching_info[j].bounding_boxes.right_bottom.y) {

				CurrentTextInformation dataset;
				dataset.center_position = this_center;

				dataset.bounding_boxes.left_top.x = iter->left_top.x;
				dataset.bounding_boxes.left_top.y = iter->left_top.y;
				dataset.bounding_boxes.right_top.x = iter->right_top.x;
				dataset.bounding_boxes.right_top.y = iter->right_top.y;
				dataset.bounding_boxes.right_bottom.x = iter->right_bottom.x;
				dataset.bounding_boxes.right_bottom.y = iter->right_bottom.y;
				dataset.bounding_boxes.left_bottom.x = iter->left_bottom.x;
				dataset.bounding_boxes.left_bottom.y = iter->left_bottom.y;

				dataset.object_id = g_object_teaching_info[j].object_id;
				dataset.reel_no = g_object_teaching_info[j].reel_no;
				dataset.label_no = g_object_teaching_info[j].label_no;
				dataset.image_name = g_image_path;

				object_info.push_back(dataset);
				iter = detection_result.erase(iter);
			}
			else {
				++iter;
			}
		}
		same_region_object_info.push_back(object_info);
	}

	return same_region_object_info;
}

std::vector<NepesInterface::RectanglePosition> GetTextInfoMergePosition(std::vector<std::vector<CurrentTextInformation>> same_line_text_info, std::vector<DetailDetectionStruct>& merge_result)
{
	std::vector<NepesInterface::RectanglePosition> average_vector;

	for (int i = 0; i < same_line_text_info.size(); ++i) {

		if (same_line_text_info[i].size() <= 0) {
			continue;
		}

		DetailDetectionStruct upgrade_detection;
		upgrade_detection.current_text_info.bounding_boxes.left_top.x = 9999;
		upgrade_detection.current_text_info.bounding_boxes.left_top.y = 9999;
		upgrade_detection.current_text_info.bounding_boxes.right_bottom.x = 0;
		upgrade_detection.current_text_info.bounding_boxes.right_bottom.y = 0;

		for (int j = 0; j < same_line_text_info[i].size(); ++j) {
			upgrade_detection.current_text_info.bounding_boxes.left_top.x = min(upgrade_detection.current_text_info.bounding_boxes.left_top.x, same_line_text_info[i][j].bounding_boxes.left_top.x);
			upgrade_detection.current_text_info.bounding_boxes.left_top.y = min(upgrade_detection.current_text_info.bounding_boxes.left_top.y, same_line_text_info[i][j].bounding_boxes.left_top.y);
			upgrade_detection.current_text_info.bounding_boxes.right_bottom.x = max(upgrade_detection.current_text_info.bounding_boxes.right_bottom.x, same_line_text_info[i][j].bounding_boxes.right_bottom.x);
			upgrade_detection.current_text_info.bounding_boxes.right_bottom.y = max(upgrade_detection.current_text_info.bounding_boxes.right_bottom.y, same_line_text_info[i][j].bounding_boxes.right_bottom.y);
			upgrade_detection.current_text_info.bounding_boxes.left_bottom.x = upgrade_detection.current_text_info.bounding_boxes.left_top.x;
			upgrade_detection.current_text_info.bounding_boxes.left_bottom.y = upgrade_detection.current_text_info.bounding_boxes.right_bottom.y;
			upgrade_detection.current_text_info.bounding_boxes.right_top.x = upgrade_detection.current_text_info.bounding_boxes.right_bottom.x;
			upgrade_detection.current_text_info.bounding_boxes.right_top.y = upgrade_detection.current_text_info.bounding_boxes.left_top.y;

			upgrade_detection.current_text_info.object_id = same_line_text_info[i][j].object_id;
			upgrade_detection.current_text_info.center_position.x = (upgrade_detection.current_text_info.bounding_boxes.left_top.x + upgrade_detection.current_text_info.bounding_boxes.right_top.x) / 2;
			upgrade_detection.current_text_info.center_position.y = (upgrade_detection.current_text_info.bounding_boxes.right_top.y + upgrade_detection.current_text_info.bounding_boxes.right_bottom.y) / 2;
			upgrade_detection.current_text_info.reel_no = same_line_text_info[i][j].reel_no;
			upgrade_detection.current_text_info.label_no = same_line_text_info[i][j].label_no;
			upgrade_detection.current_text_info.image_name = g_image_path;
			upgrade_detection.current_text_info.item_name = same_line_text_info[i][j].item_name;

			CurrentTextInformation current_text_info = same_line_text_info[i][j];
			upgrade_detection.rct_vector.push_back(current_text_info);
		}

		std::sort(upgrade_detection.rct_vector.begin(), upgrade_detection.rct_vector.end(), SortMinXTempDetectionResult);

		merge_result.push_back(upgrade_detection);
	}


	for (int i = 0; i < g_current_text_info.size(); ++i) {

		for (int j = 0; j < merge_result.size(); ++j) {

			if (g_current_text_info[i].object_id == merge_result[j].current_text_info.object_id) {

				NepesInterface::RectanglePosition diff_;
				diff_.left_top.y = g_current_text_info[i].bounding_boxes.left_top.y - merge_result[j].current_text_info.bounding_boxes.left_top.y;
				diff_.left_bottom.y = g_current_text_info[i].bounding_boxes.left_bottom.y - merge_result[j].current_text_info.bounding_boxes.left_bottom.y;
				diff_.left_top.x = g_current_text_info[i].bounding_boxes.left_top.x - merge_result[j].current_text_info.bounding_boxes.left_top.x;
				diff_.right_top.x = g_current_text_info[i].bounding_boxes.right_top.x - merge_result[j].current_text_info.bounding_boxes.right_top.x;

				average_vector.push_back(diff_);
			}

		}
	}

	return average_vector;
}


std::vector<NepesInterface::RectanglePosition> GetObjectInfoMergePosition(std::vector<std::vector<CurrentTextInformation>> same_region_object_info, std::vector<DetailDetectionStruct>& merge_result)
{
	std::vector<NepesInterface::RectanglePosition> average_vector;

	for (int i = 0; i < same_region_object_info.size(); ++i) {

		if (same_region_object_info[i].size() <= 0) {
			continue;
		}

		//CurrentTextInformation dataset;
		//dataset.bounding_boxes.left_top.x = 9999;
		//dataset.bounding_boxes.left_top.y = 9999;
		//dataset.bounding_boxes.right_bottom.x = 0;
		//dataset.bounding_boxes.right_bottom.y = 0;

		DetailDetectionStruct upgrade_detection;
		upgrade_detection.current_text_info.bounding_boxes.left_top.x = 9999;
		upgrade_detection.current_text_info.bounding_boxes.left_top.y = 9999;
		upgrade_detection.current_text_info.bounding_boxes.right_bottom.x = 0;
		upgrade_detection.current_text_info.bounding_boxes.right_bottom.y = 0;

		for (int j = 0; j < same_region_object_info[i].size(); ++j) {
			upgrade_detection.current_text_info.bounding_boxes.left_top.x = min(upgrade_detection.current_text_info.bounding_boxes.left_top.x, same_region_object_info[i][j].bounding_boxes.left_top.x);
			upgrade_detection.current_text_info.bounding_boxes.left_top.y = min(upgrade_detection.current_text_info.bounding_boxes.left_top.y, same_region_object_info[i][j].bounding_boxes.left_top.y);
			upgrade_detection.current_text_info.bounding_boxes.right_bottom.x = max(upgrade_detection.current_text_info.bounding_boxes.right_bottom.x, same_region_object_info[i][j].bounding_boxes.right_bottom.x);
			upgrade_detection.current_text_info.bounding_boxes.right_bottom.y = max(upgrade_detection.current_text_info.bounding_boxes.right_bottom.y, same_region_object_info[i][j].bounding_boxes.right_bottom.y);
			upgrade_detection.current_text_info.bounding_boxes.left_bottom.x = upgrade_detection.current_text_info.bounding_boxes.left_top.x;
			upgrade_detection.current_text_info.bounding_boxes.left_bottom.y = upgrade_detection.current_text_info.bounding_boxes.right_bottom.y;
			upgrade_detection.current_text_info.bounding_boxes.right_top.x = upgrade_detection.current_text_info.bounding_boxes.right_bottom.x;
			upgrade_detection.current_text_info.bounding_boxes.right_top.y = upgrade_detection.current_text_info.bounding_boxes.left_top.y;

			upgrade_detection.current_text_info.object_id = same_region_object_info[i][j].object_id;
			upgrade_detection.current_text_info.center_position.x = (upgrade_detection.current_text_info.bounding_boxes.left_top.x + upgrade_detection.current_text_info.bounding_boxes.right_top.x) / 2;
			upgrade_detection.current_text_info.center_position.y = (upgrade_detection.current_text_info.bounding_boxes.right_top.y + upgrade_detection.current_text_info.bounding_boxes.right_bottom.y) / 2;
			upgrade_detection.current_text_info.reel_no = same_region_object_info[i][j].reel_no;
			upgrade_detection.current_text_info.label_no = same_region_object_info[i][j].label_no;
			upgrade_detection.current_text_info.image_name = g_image_path;

			CurrentTextInformation current_text_info = same_region_object_info[i][j];
			upgrade_detection.rct_vector.push_back(current_text_info);
		}

		std::sort(upgrade_detection.rct_vector.begin(), upgrade_detection.rct_vector.end(), SortMinXTempDetectionResult);

		merge_result.push_back(upgrade_detection);
	}


	for (int i = 0; i < g_object_teaching_info.size(); ++i) {

		for (int j = 0; j < merge_result.size(); ++j) {

			if (g_object_teaching_info[i].object_id == merge_result[j].current_text_info.object_id) {

				NepesInterface::RectanglePosition diff_;
				diff_.left_top.y = g_object_teaching_info[i].bounding_boxes.left_top.y - merge_result[j].current_text_info.bounding_boxes.left_top.y;
				diff_.left_bottom.y = g_object_teaching_info[i].bounding_boxes.left_bottom.y - merge_result[j].current_text_info.bounding_boxes.left_bottom.y;
				diff_.left_top.x = g_object_teaching_info[i].bounding_boxes.left_top.x - merge_result[j].current_text_info.bounding_boxes.left_top.x;
				diff_.right_top.x = g_object_teaching_info[i].bounding_boxes.right_top.x - merge_result[j].current_text_info.bounding_boxes.right_top.x;

				average_vector.push_back(diff_);
			}

		}
	}

	return average_vector;
}



std::vector<JudgeInfo> GetSameLinePositionFromDegree(std::vector<NepesInterface::RectanglePosition>& detection_result, std::vector<CurrentTextInformation>& merge_result)
{
	std::vector<JudgeInfo> judge_vector;

	for (int i = 0; i < detection_result.size(); ++i) {
		for (int j = 0; j < merge_result.size(); ++j) {
			NepesInterface::Vertex this_center;

			this_center.x = (detection_result[i].left_top.x + detection_result[i].right_top.x) / 2;
			this_center.y = (detection_result[i].right_top.y + detection_result[i].right_bottom.y) / 2;

			double degree = atan2(merge_result[j].center_position.y - this_center.y,
				merge_result[j].center_position.x - this_center.x) * 180. / 3.141592;

			//degree = abs(degree);

			//std::cout << "몇도일까요 : " << degree << std::endl;

			if ((degree >= 0. && degree <= 1.) || (degree <= 180. && degree >= 179.) || (degree <= 0. && degree >= -1.) || (degree >= -180. && degree <= -179.)) {
				double x2 = ((double)merge_result[j].center_position.x - (double)this_center.x) * ((double)merge_result[j].center_position.x - (double)this_center.x);
				double y2 = ((double)merge_result[j].center_position.y - (double)this_center.y) * ((double)merge_result[j].center_position.y - (double)this_center.y);
				double distance = sqrt(x2 + y2);

				JudgeInfo info;
				info.object_id = merge_result[j].object_id;
				info.degree = degree;
				info.distance = distance;
				info.position.left_top.x = detection_result[i].left_top.x;
				info.position.left_top.y = detection_result[i].left_top.y;
				info.position.right_top.x = detection_result[i].right_top.x;
				info.position.right_top.y = detection_result[i].right_top.y;
				info.position.right_bottom.x = detection_result[i].right_bottom.x;
				info.position.right_bottom.y = detection_result[i].right_bottom.y;
				info.position.left_bottom.x = detection_result[i].left_bottom.x;
				info.position.left_bottom.y = detection_result[i].left_bottom.y;

				judge_vector.push_back(info);

				//std::cout << "Object ID : " << info.object_id << "Degree : " << degree << std::endl;
			}
		}
	}

	return judge_vector;
}

void GetMergePositionFromYDifference(std::vector<std::vector<CurrentTextInformation>> same_line_text_info, std::vector<DetailDetectionStruct>& merge_result)
{
	if (!merge_result.size()) {
		return;
	}

	std::vector<DetailDetectionStruct> temp_vector;

	for (int i = 0; i < same_line_text_info.size(); ++i) {

		if (same_line_text_info[i].size() <= 0) {
			continue;
		}

		//CurrentTextInformation dataset;
		//dataset.bounding_boxes.left_top.x = 9999;
		//dataset.bounding_boxes.left_top.y = 9999;
		//dataset.bounding_boxes.right_bottom.x = 0;
		//dataset.bounding_boxes.right_bottom.y = 0;

		DetailDetectionStruct upgrade_detection;
		upgrade_detection.current_text_info.bounding_boxes.left_top.x = 9999;
		upgrade_detection.current_text_info.bounding_boxes.left_top.y = 9999;
		upgrade_detection.current_text_info.bounding_boxes.right_bottom.x = 0;
		upgrade_detection.current_text_info.bounding_boxes.right_bottom.y = 0;

		for (int j = 0; j < same_line_text_info[i].size(); ++j) {
			upgrade_detection.current_text_info.bounding_boxes.left_top.x = min(upgrade_detection.current_text_info.bounding_boxes.left_top.x, same_line_text_info[i][j].bounding_boxes.left_top.x);
			upgrade_detection.current_text_info.bounding_boxes.left_top.y = min(upgrade_detection.current_text_info.bounding_boxes.left_top.y, same_line_text_info[i][j].bounding_boxes.left_top.y);
			upgrade_detection.current_text_info.bounding_boxes.right_bottom.x = max(upgrade_detection.current_text_info.bounding_boxes.right_bottom.x, same_line_text_info[i][j].bounding_boxes.right_bottom.x);
			upgrade_detection.current_text_info.bounding_boxes.right_bottom.y = max(upgrade_detection.current_text_info.bounding_boxes.right_bottom.y, same_line_text_info[i][j].bounding_boxes.right_bottom.y);
			upgrade_detection.current_text_info.bounding_boxes.left_bottom.x = upgrade_detection.current_text_info.bounding_boxes.left_top.x;
			upgrade_detection.current_text_info.bounding_boxes.left_bottom.y = upgrade_detection.current_text_info.bounding_boxes.right_bottom.y;
			upgrade_detection.current_text_info.bounding_boxes.right_top.x = upgrade_detection.current_text_info.bounding_boxes.right_bottom.x;
			upgrade_detection.current_text_info.bounding_boxes.right_top.y = upgrade_detection.current_text_info.bounding_boxes.left_top.y;

			upgrade_detection.current_text_info.object_id = same_line_text_info[i][j].object_id;
			upgrade_detection.current_text_info.center_position.x = (upgrade_detection.current_text_info.bounding_boxes.left_top.x + upgrade_detection.current_text_info.bounding_boxes.right_top.x) / 2;
			upgrade_detection.current_text_info.center_position.y = (upgrade_detection.current_text_info.bounding_boxes.right_top.y + upgrade_detection.current_text_info.bounding_boxes.right_bottom.y) / 2;
			upgrade_detection.current_text_info.reel_no = same_line_text_info[i][j].reel_no;
			upgrade_detection.current_text_info.label_no = same_line_text_info[i][j].label_no;
			upgrade_detection.current_text_info.image_name = g_image_path;
			upgrade_detection.current_text_info.item_name = same_line_text_info[i][j].item_name;

			CurrentTextInformation current_text_info = same_line_text_info[i][j];
			upgrade_detection.rct_vector.push_back(current_text_info);
		}

		//2020-11-15 JHLEE
		//merge 좌표 합쳐야됨
		for (auto iter = merge_result.begin(); iter != merge_result.end();) {

			DetailDetectionStruct result_dataset;

			if (iter->current_text_info.object_id == upgrade_detection.current_text_info.object_id) {

				result_dataset.current_text_info.bounding_boxes.left_top.x = min(upgrade_detection.current_text_info.bounding_boxes.left_top.x, iter->current_text_info.bounding_boxes.left_top.x);
				result_dataset.current_text_info.bounding_boxes.left_top.y = min(upgrade_detection.current_text_info.bounding_boxes.left_top.y, iter->current_text_info.bounding_boxes.left_top.y);
				result_dataset.current_text_info.bounding_boxes.right_bottom.x = max(upgrade_detection.current_text_info.bounding_boxes.right_bottom.x, iter->current_text_info.bounding_boxes.right_bottom.x);
				result_dataset.current_text_info.bounding_boxes.right_bottom.y = max(upgrade_detection.current_text_info.bounding_boxes.right_bottom.y, iter->current_text_info.bounding_boxes.right_bottom.y);
				result_dataset.current_text_info.bounding_boxes.left_bottom.x = result_dataset.current_text_info.bounding_boxes.left_top.x;
				result_dataset.current_text_info.bounding_boxes.left_bottom.y = result_dataset.current_text_info.bounding_boxes.right_bottom.y;
				result_dataset.current_text_info.bounding_boxes.right_top.x = result_dataset.current_text_info.bounding_boxes.right_bottom.x;
				result_dataset.current_text_info.bounding_boxes.right_top.y = result_dataset.current_text_info.bounding_boxes.left_top.y;

				result_dataset.current_text_info.object_id = iter->current_text_info.object_id;
				result_dataset.current_text_info.center_position.x = (result_dataset.current_text_info.bounding_boxes.left_top.x + result_dataset.current_text_info.bounding_boxes.right_top.x) / 2;
				result_dataset.current_text_info.center_position.y = (result_dataset.current_text_info.bounding_boxes.right_top.y + result_dataset.current_text_info.bounding_boxes.right_bottom.y) / 2;
				result_dataset.current_text_info.reel_no = iter->current_text_info.reel_no;
				result_dataset.current_text_info.label_no = iter->current_text_info.label_no;
				result_dataset.current_text_info.image_name = g_image_path;
				result_dataset.current_text_info.item_name = iter->current_text_info.item_name;

				for (int vector_index = 0; vector_index < iter->rct_vector.size(); ++vector_index) {
					CurrentTextInformation detection_result = iter->rct_vector[vector_index];
					result_dataset.rct_vector.push_back(detection_result);
				}

				iter = merge_result.erase(iter);

				temp_vector.push_back(result_dataset);
			}
			else {
				++iter;
			}
		}

	}

	for (int i = 0; i < temp_vector.size(); ++i) {
		DetailDetectionStruct data = temp_vector[i];
		std::sort(data.rct_vector.begin(), data.rct_vector.end(), SortMinXTempDetectionResult);
		merge_result.push_back(data);
	}
}

web::uri_builder GetBuilder(std::wstring query_path, std::vector<std::pair<std::wstring, std::wstring>>* pv_query)
{
	web::uri_builder builder;

	if (!query_path.empty()) {

		builder.set_path(query_path);

		if (!pv_query->empty()) {

			for (std::pair<std::wstring, std::wstring> pQuery : (*pv_query)) {
				builder.append_query(pQuery.first, pQuery.second);
			}
		}
	}

	return builder;
}

web::json::value GetJson(std::wstring url, std::wstring query_path = U(""), std::vector<std::pair<std::wstring, std::wstring>> *previous_query = nullptr)
{
	web::json::value result_json_data;
	web::http::client::http_client client(url);
	web::uri_builder builder = GetBuilder(query_path, previous_query);

	pplx::task<void> requestTask = client.request(web::http::methods::GET, builder.to_string())
		.then([&](web::http::http_response response) {
			return response.extract_json();
			})
		.then([&](pplx::task<web::json::value> previousTask) {
				try {
					result_json_data = previousTask.get();
				}
				catch (const web::http::http_exception& e) {
					std::cout << "Error exception: " << e.what() << std::endl;
				}
			});

			try {
				requestTask.wait();
			}
			catch (const std::exception& e) {
				std::cout << "Error exception: " << e.what() << std::endl;
			}

	return result_json_data;
}

std::string SetUrlParameter(std::string url, std::string parameter_name, std::string parameter_value, bool is_exist_next_parameter)
{
	std::string parameter = parameter_name + "=" +parameter_value;

	url += parameter;

	if (is_exist_next_parameter) {
		url += "&";
	}

	return url;
}

std::string WStringToString(std::wstring src)
{
	USES_CONVERSION;
	return std::string(W2A(src.c_str()));
}

void ConvertRecipeTypeJsonToStruct(_In_ web::json::value json_records, _Out_ WrapperModelInformation& model_information)
{
	std::string data = WStringToString(json_records.at(GetRecipeTypeKey(MESRecipeTypeKey::NEPES_MES_KEY_SIDE_TYPE)).as_string().c_str());
	if (data == "단면") {
		model_information.type = NepesInterface::ModelType::TYPE_ONE_SIDE;
		g_model_type = NepesInterface::ModelType::TYPE_ONE_SIDE;
	}
	else {
		model_information.type = NepesInterface::ModelType::TYPE_DOUBLE_SIDE;
		g_model_type = NepesInterface::ModelType::TYPE_DOUBLE_SIDE;
	}

	data = WStringToString(json_records.at(GetRecipeTypeKey(MESRecipeTypeKey::NEPES_MES_KEY_RECIPE)).as_string().c_str());
	model_information.recipe = data;

	data = WStringToString(json_records.at(GetRecipeTypeKey(MESRecipeTypeKey::NEPES_MES_KEY_CUSTOMER)).as_string().c_str());
	model_information.customer = data;

	//양면 시퀀스를 위해 알고 있어야 함
	g_model_type = model_information.type;
}

void ConvertReelInformationJsonToVector(web::json::value json_records)
{
	std::string data;

	for (int i = 0; i < json_records.size(); ++i) {

		MESLabelInformation label_info;

		web::json::value json_array = json_records.as_array().at(i);

		//data = WStringToString(json_array.at(GetMESRecipeInformationKey(MESRecipeInformationKey::NEPES_MES_KEY_SPEC_ID)).as_string().c_str());
		data = WStringToString(json_array.at(GetMESRecipeInformationKey(MESRecipeInformationKey::NEPES_MES_KEY_EXCEPTION)).as_string().c_str());
		if (data == "N") {
			label_info.exception = false;
		}
		else {
			label_info.exception = true;
		}

		data = WStringToString(json_array.at(GetMESRecipeInformationKey(MESRecipeInformationKey::NEPES_MES_KEY_TOP_LEFT_Y)).as_string().c_str());
		label_info.teaching_position.left_top.y = std::stoi(data);

		data = WStringToString(json_array.at(GetMESRecipeInformationKey(MESRecipeInformationKey::NEPES_MES_KEY_BOTTOM_RIGHT_X)).as_string().c_str());
		label_info.teaching_position.right_bottom.x = std::stoi(data);

		data = WStringToString(json_array.at(GetMESRecipeInformationKey(MESRecipeInformationKey::NEPES_MES_KEY_TOP_LEFT_X)).as_string().c_str());
		label_info.teaching_position.left_top.x = std::stoi(data);

		data = WStringToString(json_array.at(GetMESRecipeInformationKey(MESRecipeInformationKey::NEPES_MES_KEY_BOTTOM_LEFT_X)).as_string().c_str());
		label_info.teaching_position.left_bottom.x = std::stoi(data);

		data = WStringToString(json_array.at(GetMESRecipeInformationKey(MESRecipeInformationKey::NEPES_MES_KEY_BOTTOM_RIGHT_Y)).as_string().c_str());
		label_info.teaching_position.right_bottom.y = std::stoi(data);

		//data = WStringToString(json_array.at(GetMESRecipeInformationKey(MESRecipeInformationKey::NEPES_MES_KEY_ITEM_SEQ)).as_string().c_str());
		data = WStringToString(json_array.at(GetMESRecipeInformationKey(MESRecipeInformationKey::NEPES_MES_KEY_CONTENT)).as_string().c_str());
		label_info.content = data;

		//data = WStringToString(json_array.at(GetMESRecipeInformationKey(MESRecipeInformationKey::NEPES_MES_KEY_ITEM_ID)).as_string().c_str());
		data = WStringToString(json_array.at(GetMESRecipeInformationKey(MESRecipeInformationKey::NEPES_MES_KEY_ITEM_NAME)).as_string().c_str());
		label_info.item_name = data;

		//data = WStringToString(json_array.at(GetMESRecipeInformationKey(MESRecipeInformationKey::NEPES_MES_KEY_LABEL_ID)).as_string().c_str());
		data = WStringToString(json_array.at(GetMESRecipeInformationKey(MESRecipeInformationKey::NEPES_MES_KEY_TOP_RIGHT_X)).as_string().c_str());
		label_info.teaching_position.right_top.x = std::stoi(data);

		data = WStringToString(json_array.at(GetMESRecipeInformationKey(MESRecipeInformationKey::NEPES_MES_KEY_TOP_RIGHT_Y)).as_string().c_str());
		label_info.teaching_position.right_top.y = std::stoi(data);

		data = WStringToString(json_array.at(GetMESRecipeInformationKey(MESRecipeInformationKey::NEPES_MES_KEY_ROI_NUMBER)).as_string().c_str());
		label_info.roi_name = data;

		data = WStringToString(json_array.at(GetMESRecipeInformationKey(MESRecipeInformationKey::NEPES_MES_KEY_REEL_NUMBER)).as_string().c_str());
		label_info.reel_number = std::stoi(data);

		data = WStringToString(json_array.at(GetMESRecipeInformationKey(MESRecipeInformationKey::NEPES_MES_KEY_LABEL_SEQ)).as_string().c_str());
		label_info.label_seq = std::stoi(data);

		data = WStringToString(json_array.at(GetMESRecipeInformationKey(MESRecipeInformationKey::NEPES_MES_KEY_LABEL_NAME)).as_string().c_str());
		label_info.label_name = data;

		data = WStringToString(json_array.at(GetMESRecipeInformationKey(MESRecipeInformationKey::NEPES_MES_KEY_BOTTOM_Y)).as_string().c_str());
		label_info.teaching_position.left_bottom.y = std::stoi(data);

		data = WStringToString(json_array.at(GetMESRecipeInformationKey(MESRecipeInformationKey::NEPES_MES_KEY_ITEM_TYPE)).as_string().c_str());
		if (data == "TEXT") {
			g_mes_text_information.push_back(label_info);
		}
		else if (data == "OBJECT") {
			g_mes_object_information.push_back(label_info);
		}
		else {
			g_mes_barcode_information.push_back(label_info);
		}
	}

}

void SaveResultToFile()
{
	time_t time_var;
	struct tm time_sturct;
	//time(&time_var);
	time_var = time(NULL);
	localtime_s(&time_sturct, &time_var);

	std::string year = std::to_string(time_sturct.tm_year + 1900);
	std::string month = std::to_string(time_sturct.tm_mon + 1);
	std::string day = std::to_string(time_sturct.tm_mday);

	std::string file_name = year + "-" + month + "-" + day + ".csv";


	std::wstring w_root_path = L"D:\\Result";
	std::string root_path;
	root_path.assign(w_root_path.begin(), w_root_path.end());

	std::string full_path = root_path + "\\" + file_name;

	_wmkdir(w_root_path.c_str());

	std::ifstream input_file;
	input_file.open(full_path);

	//int total_reel_count = GetTotalReelCountFromFile();
	//int total_lot_count = GetTotalLotCountFromFile();

	if (!input_file) {
		input_file.close();

		std::ofstream output_file(full_path, std::ios::app);
		output_file << "Total Reel Count" << "," << "Total Lot Count" << "," << "Lot No" << "," << "Reel No" << "," << "Label No" << "," << "Type" << "," << 
			"Result" << "," << "AI Inference" << "," << "MES Data" << "," << "File Name" << "," << "MES Left Top" << "," << "MES Right Top" << "," 
			<< "MES Right Bottom" << "," << "MES Left Bottom" << std::endl;

		output_file.close();

		//2020-11-18 JHLEE 추가
		//std::string total_reel_count_file_name = "ReelCount.txt";
		//std::ofstream output_file_total_reel_count(total_reel_count_file_name);

		//output_file_total_reel_count << 1;
		g_reel_total_count = 1;

		//output_file_total_reel_count.close();

		//std::string total_lot_count_file_name = "LotCount.txt";
		//std::ofstream output_file_total_lot_count(total_lot_count_file_name);

		//output_file_total_lot_count << 1;
		g_lot_total_count = 1;

		//output_file_total_lot_count.close();

		//total_reel_count = 1;
		//total_lot_count = 1;
	}

	std::ofstream output_file(full_path, std::ios::app);

	for (int i = 0; i < g_output_result_data.size(); ++i) {

		std::string type;
		std::string pass;

		if (g_output_result_data[i].type == NepesInterface::DataType::TYPE_TEXT) {
			type = "TEXT";
		}
		else {
			type = "BARCODE";
		}

		if (g_output_result_data[i].pass) {
			pass = "PASS";
		}
		else {
			pass = "FAIL";
		}

		std::string teaching_left_top;
		std::string teaching_right_top;
		std::string teaching_right_bottom;
		std::string teaching_left_bottom;

		std::string teaching_pos_x_string;
		std::string teaching_pos_y_string;

		int teaching_pos_x = g_output_result_data[i].teaching_left_top.x;
		int teaching_pos_y = g_output_result_data[i].teaching_left_top.y;

		teaching_pos_x_string = std::to_string(teaching_pos_x);
		teaching_pos_y_string = std::to_string(teaching_pos_y);

		teaching_left_top = teaching_pos_x_string + "/" + teaching_pos_y_string;

		teaching_pos_x = g_output_result_data[i].teaching_right_top.x;
		teaching_pos_y = g_output_result_data[i].teaching_right_top.y;

		teaching_pos_x_string = std::to_string(teaching_pos_x);
		teaching_pos_y_string = std::to_string(teaching_pos_y);

		teaching_right_top = teaching_pos_x_string + "/" + teaching_pos_y_string;

		teaching_pos_x = g_output_result_data[i].teaching_right_bottom.x;
		teaching_pos_y = g_output_result_data[i].teaching_right_bottom.y;

		teaching_pos_x_string = std::to_string(teaching_pos_x);
		teaching_pos_y_string = std::to_string(teaching_pos_y);

		teaching_right_bottom = teaching_pos_x_string + "/" + teaching_pos_y_string;

		teaching_pos_x = g_output_result_data[i].teaching_left_bottom.x;
		teaching_pos_y = g_output_result_data[i].teaching_left_bottom.y;

		teaching_pos_x_string = std::to_string(teaching_pos_x);
		teaching_pos_y_string = std::to_string(teaching_pos_y);

		teaching_left_bottom = teaching_pos_x_string + "/" + teaching_pos_y_string;

		//2021-01-29 JHLEE
		//이거 확인해보고 수정해서 다시 적용하자
		std::string error_type = g_output_result_data[i].error_type;
		output_file << g_reel_total_count << "," << g_lot_total_count << "," << g_lot_number << "," << g_output_result_data[i].reel_no << "," << g_output_result_data[i].label_no <<
			"," << type << "," << error_type << "," << g_output_result_data[i].result_data << "," << g_output_result_data[i].mes_data << "," <<
			g_output_result_data[i].image_name << "," << teaching_left_top << "," << teaching_right_top << "," << teaching_right_bottom << "," << teaching_left_bottom << std::endl;

		//output_file << total_count << "," << g_lot_number << "," << g_temp_output_result_data[i].reel_no << "," << g_temp_output_result_data[i].label_no <<
		//	"," << type << "," << pass << "," << g_temp_output_result_data[i].result_data << "," << g_temp_output_result_data[i].mes_data << "," << 
		//	g_temp_output_result_data[i].image_name << std::endl;
	}

	//for (int i = 0; i < g_temp_output_result_data.size(); ++i) {

	//	std::string type;
	//	std::string pass;

	//	if (g_temp_output_result_data[i].type == NepesInterface::DataType::TYPE_TEXT) {
	//		type = "TEXT";
	//	}
	//	else {
	//		type = "BARCODE";
	//	}

	//	if (g_temp_output_result_data[i].pass) {
	//		pass = "PASS";
	//	}
	//	else {
	//		pass = "FAIL";
	//	}

	//	std::string teaching_left_top;
	//	std::string teaching_right_top;
	//	std::string teaching_right_bottom;
	//	std::string teaching_left_bottom;

	//	std::string teaching_pos_x_string;
	//	std::string teaching_pos_y_string;

	//	int teaching_pos_x = g_temp_output_result_data[i].teaching_left_top.x;
	//	int teaching_pos_y = g_temp_output_result_data[i].teaching_left_top.y;

	//	teaching_pos_x_string = std::to_string(teaching_pos_x);
	//	teaching_pos_y_string = std::to_string(teaching_pos_y);

	//	teaching_left_top = teaching_pos_x_string + "/" + teaching_pos_y_string;

	//	teaching_pos_x = g_temp_output_result_data[i].teaching_right_top.x;
	//	teaching_pos_y = g_temp_output_result_data[i].teaching_right_top.y;

	//	teaching_pos_x_string = std::to_string(teaching_pos_x);
	//	teaching_pos_y_string = std::to_string(teaching_pos_y);

	//	teaching_right_top = teaching_pos_x_string + "/" + teaching_pos_y_string;

	//	teaching_pos_x = g_temp_output_result_data[i].teaching_right_bottom.x;
	//	teaching_pos_y = g_temp_output_result_data[i].teaching_right_bottom.y;

	//	teaching_pos_x_string = std::to_string(teaching_pos_x);
	//	teaching_pos_y_string = std::to_string(teaching_pos_y);

	//	teaching_right_bottom = teaching_pos_x_string + "/" + teaching_pos_y_string;

	//	teaching_pos_x = g_temp_output_result_data[i].teaching_left_bottom.x;
	//	teaching_pos_y = g_temp_output_result_data[i].teaching_left_bottom.y;

	//	teaching_pos_x_string = std::to_string(teaching_pos_x);
	//	teaching_pos_y_string = std::to_string(teaching_pos_y);

	//	teaching_left_bottom = teaching_pos_x_string + "/" + teaching_pos_y_string;

	//	//output_file << total_count << "," << g_lot_number << "," << g_temp_output_result_data[i].reel_no << "," << g_temp_output_result_data[i].label_no <<
	//	//	"," << type << "," << pass << "," << g_temp_output_result_data[i].result_data << "," << g_temp_output_result_data[i].mes_data << "," << 
	//	//	g_temp_output_result_data[i].image_name << std::endl;

	//	output_file << total_reel_count << "," << total_lot_count << "," << g_lot_number << "," << g_temp_output_result_data[i].reel_no << "," << g_temp_output_result_data[i].label_no <<
	//		"," << type << "," << g_temp_output_result_data[i].error_type << "," << g_temp_output_result_data[i].result_data << "," << g_temp_output_result_data[i].mes_data << "," <<
	//		g_temp_output_result_data[i].image_name << "," << teaching_left_top << "," << teaching_right_top << "," << teaching_right_bottom << "," << teaching_left_bottom << std::endl;
	//}
}

void ClearWrapperMESResponseSet()
{
	g_mes_wrapper_response_set.status.clear();
	g_mes_wrapper_response_set.result.clear();
}

void ClearWrapperModelInformation()
{
	g_wrapper_model_information.recipe.clear();
	g_wrapper_model_information.customer.clear();
}

void ClearLabelInformation()
{
	g_mes_text_information.clear();
	g_mes_barcode_information.clear();
}

NepesInterface::VisionResultCode CheckBarcodeInMiniLabelROI(_In_ WrapperVisionROIInformation mini_label_roi_info, std::vector<WrapperBarcodeDataset>& temp_input_barcode_dataset, std::vector<WrapperBarcodeDataset>& mini_label_barcode)
{
	NepesInterface::VisionResultCode return_code = NepesInterface::VisionResultCode::NEPES_SUCCESS;

	for (auto iter = temp_input_barcode_dataset.begin(); iter != temp_input_barcode_dataset.end();) {

		//미나 라벨이 부착되는 ROI 영역내에 있는 바코드 좌표라면 -> 미니 라벨의 바코드 일 것이다.
		if (iter->center_position.x >= mini_label_roi_info.roi_region.left_top.x &&
			iter->center_position.y >= mini_label_roi_info.roi_region.left_top.y &&
			iter->center_position.x <= mini_label_roi_info.roi_region.right_bottom.x &&
			iter->center_position.y <= mini_label_roi_info.roi_region.right_bottom.y) {

			WrapperBarcodeDataset dataset;
			dataset.barcode_reading_data = iter->barcode_reading_data;
			dataset.center_position.x = iter->center_position.x;
			dataset.center_position.y = iter->center_position.y;

			mini_label_barcode.push_back(dataset);

			iter = temp_input_barcode_dataset.erase(iter);
		}
		else {
			++iter;
		}
	}
	return return_code;
}

bool CheckExceptionInformation(_In_ int inference_index, _In_ int teaching_index, _In_ std::string inference_text, _In_ std::string mes_text, _Inout_ NepesInterface::ResultDataset& result_dataset)
{
	bool result = false;

	do
	{
		std::string temp_inference_data = inference_text;
		std::string temp_mes_data = mes_text;

		//애매한 대 소문자 처리
		for (int temp_index = 0; temp_index < temp_inference_data.size(); ++temp_index) {
			temp_inference_data[temp_index] = tolower(temp_inference_data[temp_index]);
		}

		for (int temp_index = 0; temp_index < temp_mes_data.size(); ++temp_index) {
			temp_mes_data[temp_index] = tolower(temp_mes_data[temp_index]);
		}

		if (temp_inference_data == temp_mes_data) {
			result_dataset.pass = true;
			result_dataset.error_type = g_ai_result_type[(int)AIResultType::NEPES_AI_RESULT_PASS].c_str();

			g_text_inference_result[inference_index].text_data = g_current_text_info[teaching_index].text_data;
			g_text_inference_result[inference_index].text_data.erase(
				std::remove(g_text_inference_result[inference_index].text_data.begin(), g_text_inference_result[inference_index].text_data.end(), ' '), 
				g_text_inference_result[inference_index].text_data.end());

			result_dataset.result_data = g_text_inference_result[inference_index].text_data.c_str();
		}
		else {

			//폰트 구분 불가능 시 처리

			std::string item_name = g_current_text_info[teaching_index].item_name;

			int index = item_name.find("JSCK_E");
			if (index < 0) {
				result_dataset.pass = false;
				result_dataset.error_type = g_ai_result_type[(int)AIResultType::NEPES_AI_RESULT_FAIL_MATCHING].c_str();
				result_dataset.result_data = g_text_inference_result[inference_index].text_data.c_str();
				break;
			}

			//로직
			std::string temp_inference_reel_id = inference_text;
			std::string temp_mes_reel_id = mes_text;

			int real_index = -1;

			int index_1 = (int)temp_inference_reel_id.find("ReelID");
			if (index_1 >= 0) {
				real_index = index_1;
			}
			int index_2 = (int)temp_inference_reel_id.find("Reel1D");
			if (index_2 >= 0) {
				real_index = index_2;
			}

			int index_3 = (int)temp_inference_reel_id.find("ReellD");
			if (index_3 >= 0) {
				real_index = index_3;
			}

			int index_4 = (int)temp_inference_reel_id.find("Reel|D");
			if (index_4 >= 0) {
				real_index = index_4;
			}

			int index_5 = (int)temp_inference_reel_id.find("Ree1ID");
			if (index_5 >= 0) {
				real_index = index_5;
			}

			int index_6 = (int)temp_inference_reel_id.find("Ree11D");
			if (index_6 >= 0) {
				real_index = index_6;
			}

			int index_7 = (int)temp_inference_reel_id.find("Ree1lD");
			if (index_7 >= 0) {
				real_index = index_7;
			}

			int index_8 = (int)temp_inference_reel_id.find("Ree1|D");
			if (index_8 >= 0) {
				real_index = index_8;
			}

			int index_9 = (int)temp_inference_reel_id.find("Ree|ID");
			if (index_9 >= 0) {
				real_index = index_9;
			}
			
			int index_10 = (int)temp_inference_reel_id.find("Ree|1D");
			if (index_10 >= 0) {
				real_index = index_10;
			}

			int index_11 = (int)temp_inference_reel_id.find("Ree|lD");
			if (index_11 >= 0) {
				real_index = index_11;
			}

			int index_12 = (int)temp_inference_reel_id.find("Ree||D");
			if (index_12 >= 0) {
				real_index = index_12;
			}

			int index_13 = (int)temp_inference_reel_id.find("ReeIID");
			if (index_13 >= 0) {
				real_index = index_13;
			}

			int index_14 = (int)temp_inference_reel_id.find("ReeI1D");
			if (index_14 >= 0) {
				real_index = index_14;
			}

			int index_15 = (int)temp_inference_reel_id.find("ReeIlD");
			if (index_15 >= 0) {
				real_index = index_15;
			}

			int index_16 = (int)temp_inference_reel_id.find("ReeI|D");
			if (index_16 >= 0) {
				real_index = index_16;
			}

			if(real_index < 0){
				result_dataset.pass = false;
				result_dataset.error_type = g_ai_result_type[(int)AIResultType::NEPES_AI_RESULT_FAIL_MATCHING].c_str();
				result_dataset.result_data = g_text_inference_result[inference_index].text_data.c_str();
			}
			else {
				temp_inference_reel_id.replace(real_index, 6, "");
				temp_mes_reel_id.replace(0, 6, "");

				if (temp_inference_reel_id == temp_mes_reel_id) {
					result_dataset.pass = true;
					result_dataset.error_type = g_ai_result_type[(int)AIResultType::NEPES_AI_RESULT_PASS].c_str();

					g_text_inference_result[inference_index].text_data = g_current_text_info[teaching_index].text_data;
					g_text_inference_result[inference_index].text_data.erase(
						std::remove(g_text_inference_result[inference_index].text_data.begin(), g_text_inference_result[inference_index].text_data.end(), ' '),
						g_text_inference_result[inference_index].text_data.end());

					result_dataset.result_data = g_text_inference_result[inference_index].text_data.c_str();
				}
				else {
					result_dataset.pass = false;
					result_dataset.error_type = g_ai_result_type[(int)AIResultType::NEPES_AI_RESULT_FAIL_MATCHING].c_str();
					result_dataset.result_data = g_text_inference_result[inference_index].text_data.c_str();
				}

			}
		}

		result = true;

	} while (false);

	g_reel_pass &= result_dataset.pass;

	return result;
}

NEPES_INTERFACE_API void NepesInterface::StartAIInterfaceModule()
{
	if(!g_already_start_ai_module){
		g_already_start_ai_module = true;

		Py_Initialize();
		PyEval_InitThreads();
		thread_state = PyEval_SaveThread();

		//마샬링 시 문제가 될 수도 있어서 클리어 해줘야함
		//ClearWrapperMESResponseSet();

		for (int i = 0; i < (int)NepesInterface::VisionResultCode::NEPES_RESULT_CODE_TOTAL; ++i) {
			std::string message = GetVisionInterfaceMessage(i);
			g_vision_interface_message.push_back(message);
		}

		for (int i = 0; i < (int)AIResultType::NEPES_AI_RESULT_TOTAL; ++i) {
			std::string type = GetAIResultType(i);
			g_ai_result_type.push_back(type);
		}

		g_book = xlCreateBook();
	}
}

NEPES_INTERFACE_API void NepesInterface::StopAIInterfaceModule()
{
	if(g_already_start_ai_module){
		g_already_start_ai_module = false;

		PyEval_RestoreThread(thread_state);
		Py_Finalize();
	}
}

NEPES_INTERFACE_API NepesInterface::AIInterfaceResultCode NepesInterface::LoadAIModel()
{
	AIInterfaceResultCode return_code = AIInterfaceResultCode::NEPES_SUCCESS;

	py_gil = PyGILState_Ensure();

	PyObject* argument_list = nullptr;

	do {
		if (g_loaded_ai_model) {
			return_code = AIInterfaceResultCode::NEPES_ERROR_ALREADY_LOAD_AI_MODEL;
			break;
		}

		//set option
		g_ai_option = PythonFunctionCall("set_opt", return_code, nullptr);
		if (return_code != AIInterfaceResultCode::NEPES_SUCCESS) {
			break;
		}
		if (!g_ai_option) {
			return_code = AIInterfaceResultCode::NEPES_ERROR_AI_OPTION_OBJECT_NULL;
			break;
		}

		//check option
		PythonFunctionCall("call_opt", return_code, g_ai_option);
		if (return_code != AIInterfaceResultCode::NEPES_SUCCESS) {
			break;
		}

		//load detection model
		argument_list = Py_BuildValue("(O, i)", g_ai_option, 0);
		if (!argument_list) {
			return_code = AIInterfaceResultCode::NEPES_ERROR_FAILED_TO_GET_DETECTION_MODEL_ARGUMENT_LIST;
			break;
		}

		g_detection_model = PythonFunctionCall("model_load", return_code, argument_list);
		if (return_code != AIInterfaceResultCode::NEPES_SUCCESS) {
			break;
		}
		if (!g_detection_model) {
			return_code = AIInterfaceResultCode::NEPES_ERROR_AI_DETECTION_MODEL_NULL;
			break;
		}

		argument_list = Py_BuildValue("(O, i)", g_ai_option, 1);
		if (!argument_list) {
			return_code = AIInterfaceResultCode::NEPES_ERROR_FAILED_TO_GET_CLASSIFICATION_MODEL_ARGUMENT_LIST;
			break;
		}

		g_classification_model = PythonFunctionCall("model_load", return_code, argument_list);
		if (!g_classification_model) {
			return_code = AIInterfaceResultCode::NEPES_ERROR_AI_CLASSIFICATION_MODEL_NULL;
			break;
		}

		if (return_code == AIInterfaceResultCode::NEPES_SUCCESS) {
			g_loaded_ai_model = true;
		}

		

	} while (false);

	if (argument_list) {
		Py_DECREF(argument_list);
	}

	PyGILState_Release(py_gil);

	return return_code;
}

NEPES_INTERFACE_API NepesInterface::VisionResultCode NepesInterface::CheckLabelNumber(_In_ ReelPosition pos, _In_ int reel_no, _In_ int barcode_array_size, _In_ BarcodeDataset barcode_dataset[], _In_ int roi_array_size, _In_ VisionROIInformation roi_info[], _In_ ModelInformation* model_information, _Out_ Message* message)
{
	VisionResultCode return_code = VisionResultCode::NEPES_SUCCESS;

	std::ofstream roi_file("D:\\Result\\Localization.txt", std::ios::app);

	do {

		if (barcode_array_size <= 0) {
			return_code = VisionResultCode::NEPES_ERROR_INVALID_BARCODE_ARRAY_SIZE;
			message->error_message = g_vision_interface_message[(int)return_code].c_str();
			break;
		}

		if (roi_array_size <= 0) {
			return_code = VisionResultCode::NEPES_ERROR_INVALID_ROI_ARRAY_SIZE;
			message->error_message = g_vision_interface_message[(int)return_code].c_str();
			break;
		}

		if (!barcode_dataset) {
			return_code = VisionResultCode::NEPES_ERROR_BARCODE_ARRAY_POINTER_NULL;
			message->error_message = g_vision_interface_message[(int)return_code].c_str();
			break;
		}

		if (!roi_info) {
			return_code = VisionResultCode::NEPES_ERROR_ROI_ARRAY_POINTER_NULL;
			message->error_message = g_vision_interface_message[(int)return_code].c_str();
			break;
		}

		if (!model_information) {
			return_code = VisionResultCode::NEPES_ERROR_ROI_ARRAY_POINTER_NULL;
			message->error_message = g_vision_interface_message[(int)return_code].c_str();
			break;
		}

		std::string customer = model_information->customer;
		std::string recipe = model_information->recipe;

		roi_file << "Lot : " << g_lot_number << " Reel : " << reel_no <<  std::endl;
		roi_file << "Customer : " << customer << " Recipe : " << recipe << std::endl;

		std::vector<WrapperBarcodeDataset> temp_input_barcode_dataset;

		for (int i = 0; i < barcode_array_size; ++i) {
			WrapperBarcodeDataset dataset;
			dataset.center_position = barcode_dataset[i].center_position;
			dataset.barcode_reading_data = barcode_dataset[i].barcode_reading_data;

			//LT 값으로 시작하는 바코드는 No Count
			if (dataset.barcode_reading_data.find("LT") == 0) {
				continue;
			}

			//ID 값으로 시작하는 바코드는 No Count
			if (dataset.barcode_reading_data.find("ID") == 0) {
				continue;
			}

			//output_file << "Barcode : " << dataset.barcode_reading_data << std::endl;
			//output_file << "Center pos X : " << dataset.center_position.x << "Center pos Y : " << dataset.center_position.y << std::endl;

			temp_input_barcode_dataset.push_back(dataset);
		}

		std::vector<WrapperVisionROIInformation> temp_roi_info;
		WrapperVisionROIInformation mini_label_roi_info;
		std::vector<WrapperBarcodeDataset> mini_label_barcode;

		bool roi_not_found = true;

		for (int i = 0; i < roi_array_size; ++i) {
			WrapperVisionROIInformation temp_value;

			temp_value.roi_name = roi_info[i].roi_name;
			temp_value.roi_region.left_top.x = roi_info[i].rect_pos.left_top.x;
			temp_value.roi_region.left_top.y = roi_info[i].rect_pos.left_top.y;
			temp_value.roi_region.right_top.x = roi_info[i].rect_pos.right_top.x;
			temp_value.roi_region.right_top.y = roi_info[i].rect_pos.right_top.y;
			temp_value.roi_region.right_bottom.x = roi_info[i].rect_pos.right_bottom.x;
			temp_value.roi_region.right_bottom.y = roi_info[i].rect_pos.right_bottom.y;
			temp_value.roi_region.left_bottom.x = roi_info[i].rect_pos.left_bottom.x;
			temp_value.roi_region.left_bottom.y = roi_info[i].rect_pos.left_bottom.y;

			roi_file << "ROI Name : " << temp_value.roi_name << std::endl;
			roi_file << "LT X Y : " << temp_value.roi_region.left_top.x << "," << temp_value.roi_region.left_top.y << std::endl;
			roi_file << "RT X Y : " << temp_value.roi_region.right_top.x << "," << temp_value.roi_region.right_top.y << std::endl;
			roi_file << "RB X Y : " << temp_value.roi_region.right_bottom.x << "," << temp_value.roi_region.right_bottom.y << std::endl;
			roi_file << "LB X Y : " << temp_value.roi_region.left_bottom.x << "," << temp_value.roi_region.left_bottom.y << std::endl;

			if (g_model_type == ModelType::TYPE_ONE_SIDE && temp_value.roi_name == "1") {

				mini_label_roi_info.roi_name = roi_info[i].roi_name;
				mini_label_roi_info.roi_region.left_top.x = roi_info[i].rect_pos.left_top.x;
				mini_label_roi_info.roi_region.left_top.y = roi_info[i].rect_pos.left_top.y;
				mini_label_roi_info.roi_region.right_top.x = roi_info[i].rect_pos.right_top.x;
				mini_label_roi_info.roi_region.right_top.y = roi_info[i].rect_pos.right_top.y;
				mini_label_roi_info.roi_region.right_bottom.x = roi_info[i].rect_pos.right_bottom.x;
				mini_label_roi_info.roi_region.right_bottom.y = roi_info[i].rect_pos.right_bottom.y;
				mini_label_roi_info.roi_region.left_bottom.x = roi_info[i].rect_pos.left_bottom.x;
				mini_label_roi_info.roi_region.left_bottom.y = roi_info[i].rect_pos.left_bottom.y;

				roi_not_found = false;
			}
			else if (g_model_type == ModelType::TYPE_DOUBLE_SIDE && temp_value.roi_name == "1_F") {
				mini_label_roi_info.roi_name = roi_info[i].roi_name;
				mini_label_roi_info.roi_region.left_top.x = roi_info[i].rect_pos.left_top.x;
				mini_label_roi_info.roi_region.left_top.y = roi_info[i].rect_pos.left_top.y;
				mini_label_roi_info.roi_region.right_top.x = roi_info[i].rect_pos.right_top.x;
				mini_label_roi_info.roi_region.right_top.y = roi_info[i].rect_pos.right_top.y;
				mini_label_roi_info.roi_region.right_bottom.x = roi_info[i].rect_pos.right_bottom.x;
				mini_label_roi_info.roi_region.right_bottom.y = roi_info[i].rect_pos.right_bottom.y;
				mini_label_roi_info.roi_region.left_bottom.x = roi_info[i].rect_pos.left_bottom.x;
				mini_label_roi_info.roi_region.left_bottom.y = roi_info[i].rect_pos.left_bottom.y;

				roi_not_found = false;
			}

			temp_roi_info.push_back(temp_value);
		}

		//양면인 경우도 고려해야하므로 예외처리 해준다.
		if (pos == ReelPosition::POS_FRONT) {

			if (roi_not_found) {
				return_code = VisionResultCode::NEPES_ERROR_INVALID_ROI_NAME;
				message->error_message = g_vision_interface_message[(int)return_code].c_str();
				break;
			}

			return_code = CheckBarcodeInMiniLabelROI(mini_label_roi_info, temp_input_barcode_dataset, mini_label_barcode);
			if (return_code != VisionResultCode::NEPES_SUCCESS) {
				message->error_message = g_vision_interface_message[(int)return_code].c_str();
				break;
			}

			//미니라벨은 총 1D 바코드 1개, 2D 바코드 1개로 구성되어 있음
			//즉 1개나 2개가 읽혔으면 (간혹 1D를 못읽고 2D만 읽을 수도 있다! 미니라벨 바코드의 품질이 안 좋아서) 미니라벨이 존재하는 거임
			int mini_label_barcode_num = (int)mini_label_barcode.size();

			if (mini_label_barcode_num <= 0) {
				return_code = VisionResultCode::NEPES_ERROR_LABEL_OMITTED_MINI_LABEL;
				message->error_message = g_vision_interface_message[(int)return_code].c_str();
				break;
			}
			else if (mini_label_barcode_num >= 3) {
				return_code = VisionResultCode::NEPES_ERROR_LABEL_EXCEEDED_MINI_LABEL;
				message->error_message = g_vision_interface_message[(int)return_code].c_str();
				break;
			}
		}

		int mes_barcode_total_num = 0;
		int mes_mini_label_barcode_num = 0;
		int real_reel_no = reel_no + 1;

		std::string mini_roi_name = "";

		if (g_model_type == NepesInterface::ModelType::TYPE_DOUBLE_SIDE && pos == ReelPosition::POS_FRONT) {
			mini_roi_name = "1_F";
		}
		else if (g_model_type == NepesInterface::ModelType::TYPE_ONE_SIDE && pos == ReelPosition::POS_FRONT) {
			mini_roi_name = "1";
		}

		for (int j = 0; j < temp_roi_info.size(); ++j) {
			for (int i = 0; i < g_mes_barcode_information.size(); ++i) {
				//MES Barcode 데이터 벡터에서 ROI 이름과 현재 릴 번호가 일치여부 체크
				if (temp_roi_info[j].roi_name == g_mes_barcode_information[i].roi_name && real_reel_no == g_mes_barcode_information[i].reel_number) {
					//미니 라벨 바코드는 따로 카운트해서 빼야 함
					if (g_mes_barcode_information[i].roi_name == mini_roi_name) {
						++mes_mini_label_barcode_num;
					}
					++mes_barcode_total_num;
				}
			}
		}

		//비교
		int input_barcode_num = (int)temp_input_barcode_dataset.size();

		mes_barcode_total_num = mes_barcode_total_num - mes_mini_label_barcode_num;

		if (input_barcode_num > mes_barcode_total_num) {
			return_code = VisionResultCode::NEPES_ERROR_LABEL_EXCEEDED;
			message->error_message = g_vision_interface_message[(int)return_code].c_str();
		}
		else if (input_barcode_num < mes_barcode_total_num) {
			return_code = VisionResultCode::NEPES_ERROR_LABEL_OMITTED;
			message->error_message = g_vision_interface_message[(int)return_code].c_str();
		}

	} while (false);

	roi_file.close();

	return return_code;
}

NEPES_INTERFACE_API NepesInterface::AIInterfaceResultCode NepesInterface::SetImagePath(_In_ const char* image_path)
{
	AIInterfaceResultCode return_code = AIInterfaceResultCode::NEPES_SUCCESS;

	do {
		py_gil = PyGILState_Ensure();

		if (!g_loaded_ai_model) {
			return_code = AIInterfaceResultCode::NEPES_ERROR_NOT_LOAD_AI_MODEL;
			break;
		}

		PyGILState_Release(py_gil);

		if (!image_path) {
			return_code = AIInterfaceResultCode::NEPES_ERROR_INPUT_DATA_EMPTY_OR_NULL;
			break;
		}

		g_image_path.clear();

		g_image_path = image_path;

	} while (false);

	return return_code;
}

NEPES_INTERFACE_API NepesInterface::AIInterfaceResultCode NepesInterface::StartTextDetection()
{
	AIInterfaceResultCode return_code = AIInterfaceResultCode::NEPES_SUCCESS;

	//2020-01-03 Tact Time 추가
	start_total_time = clock();
	clock_t this_time = clock();

	py_gil = PyGILState_Ensure();

	PyObject* image_path = nullptr;
	PyObject* argument_list = nullptr;
	PyObject* detection_result_list = nullptr;

	do {
		if (!g_loaded_ai_model) {
			return_code = AIInterfaceResultCode::NEPES_ERROR_NOT_LOAD_AI_MODEL;
			break;
		}

		image_path = PyUnicode_FromString(g_image_path.c_str());
		argument_list = Py_BuildValue("(O, O, O)", g_ai_option, g_detection_model, image_path);

		if (!argument_list) {
			return_code = AIInterfaceResultCode::NEPES_ERROR_FAILED_TO_GET_DETECTION_RUN_ARGUMENT_LIST;
			break;
		}

		detection_result_list = PythonFunctionCall("detection_run", return_code, argument_list);

		if (!detection_result_list) {
			return_code = AIInterfaceResultCode::NEPES_ERROR_AI_DETECTION_RESULT_NULL;
			break;
		}
		
		return_code = ConvertDoubleListToVector(detection_result_list, g_detection_result);

	} while (false);

	if (image_path) {
		Py_DECREF(image_path);
	}

	if(argument_list){
		Py_DECREF(argument_list);
	}

	if (detection_result_list) {
		Py_DECREF(detection_result_list);
	}

	PyGILState_Release(py_gil);

	double this_tact_time = ((double)clock() - this_time);

	std::ofstream output_file("D:\\Result\\Log.txt", std::ios::app);

	output_file << "This Label Tact-Time Check Start!" << std::endl;
	output_file << "Lot : " << g_lot_number << " Reel : " << g_reel_total_count << " StartTextDetection() : " << this_tact_time << "ms" << std::endl;

	output_file.close();

	return return_code;
}

NEPES_INTERFACE_API NepesInterface::VisionResultCode NepesInterface::SetBarcodeReadingResult(_In_ ReelPosition pos, _In_ int array_size, _In_ BarcodeDataset barcode_dataset[])
{
	VisionResultCode return_code = VisionResultCode::NEPES_SUCCESS;

	do {

		if (array_size <= 0) {
			return_code = VisionResultCode::NEPES_ERROR_INVALID_BARCODE_ARRAY_SIZE;
			break;
		}

		if (!barcode_dataset) {
			return_code = VisionResultCode::NEPES_ERROR_BARCODE_ARRAY_POINTER_NULL;
			break;
		}

		for (int i = 0; i < array_size; ++i) {
			WrapperBarcodeDataset dataset;
			dataset.center_position = barcode_dataset[i].center_position;
			dataset.barcode_reading_data = barcode_dataset[i].barcode_reading_data;

			g_wrapper_barcode_data.push_back(dataset);
		}


	} while (false);

	return return_code;
}

NEPES_INTERFACE_API NepesInterface::AIInterfaceResultCode NepesInterface::AlignLabelDataByMESInformation(_In_ ReelPosition reel_pos, _In_ int reel_no, _In_ int current_label_index)
{
	AIInterfaceResultCode return_code = AIInterfaceResultCode::NEPES_SUCCESS;

	std::ofstream output_file3("D:\\Result\\Log3.txt", std::ios::app);
	output_file3 << "Lot : " << g_lot_number << " Reel : " << reel_no << " label_seq : " << current_label_index << " GetCurrentLabelBarcodeInformation() : " << std::endl;
	
	//2020-01-03 Tact Time 추가
	clock_t this_time = clock();

	do {
		py_gil = PyGILState_Ensure();

		if (!g_loaded_ai_model) {
			return_code = AIInterfaceResultCode::NEPES_ERROR_NOT_LOAD_AI_MODEL;
			break;
		}

		if (!g_detection_result.size()) {
			return_code = AIInterfaceResultCode::NEPES_ERROR_AI_DETECTION_RESULT_SIZE_ZERO;
			break;
		}

		PyGILState_Release(py_gil);

		g_reel_pos = reel_pos;

		std::vector<RectanglePosition> temp_detection_result = g_detection_result;

		if (!GetCurrentLabelTextInformation(reel_pos, reel_no, current_label_index)) {
			return_code = AIInterfaceResultCode::NEPES_ERROR_FAILED_TO_GET_TEXT_TEACHING_DATA;
			break;
		}
		if (!GetCurrentLabelBarcodeInformation(reel_pos, reel_no, current_label_index)) {
			return_code = AIInterfaceResultCode::NEPES_ERROR_FAILED_TO_GET_BARCODE_TEACHING_DATA;
			break;
		}
		if (!GetCurrentLabelObjectInformation(reel_pos, reel_no, current_label_index)) {
			return_code = AIInterfaceResultCode::NEPES_ERROR_FAILED_TO_GET_OBJECT_TEACHING_DATA;
			break;
		}

		std::vector<std::vector<CurrentTextInformation>> same_line_text_info;
		std::vector<std::vector<CurrentTextInformation>> same_region_object_info;
		std::vector<RectanglePosition> average_vector;

		//2021-01-14 JHLEE 수정
		if (!AlignBarcodeReadingResult(reel_no)) {
			return_code = AIInterfaceResultCode::NEPES_ERROR_FAILED_TO_ALIGN_BARCODE_DATA;
			break;
		}

		//2021-02-14 JHLEE
		//OBJECT 추가
		same_region_object_info = AlignObjectReadingResult(temp_detection_result, 0, 0);
		GetObjectInfoMergePosition(same_region_object_info, g_object_info_merge_result);

		same_line_text_info = GetInformationFromMESTeachingBox(temp_detection_result,0,0);
		average_vector = GetTextInfoMergePosition(same_line_text_info, g_detection_merge_result);

		//예외처리
		//만약 첫 번째 Merge가 실패하면...
		if (temp_detection_result.size() > 0) {

			same_line_text_info.clear();

			int left_top_y_diff = 0;
			int left_bottom_y_diff = 0;

			for (int i = 0; i < average_vector.size(); ++i) {
				left_top_y_diff += average_vector[i].left_top.y;
				left_bottom_y_diff += average_vector[i].left_bottom.y;
			}

			left_top_y_diff = (int)(left_top_y_diff / (int)average_vector.size());
			left_bottom_y_diff = (int)(left_bottom_y_diff / (int)average_vector.size());

			same_line_text_info = GetInformationFromMESTeachingBox(temp_detection_result, left_top_y_diff, left_bottom_y_diff);

			GetMergePositionFromYDifference(same_line_text_info, g_detection_merge_result);
		}

		std::sort(g_detection_merge_result.begin(), g_detection_merge_result.end(), SortTextInformationObjectID);

	} while (false);

	double this_tact_time = ((double)clock() - this_time);

	std::ofstream output_file("D:\\Result\\Log.txt", std::ios::app);

	output_file << "Lot : " << g_lot_number << " Reel : " << g_reel_total_count << " AlignLabelDataByMESInformation() : " << this_tact_time << "ms" << std::endl;

	output_file.close();

	output_file3.close();

	return return_code;
}

NEPES_INTERFACE_API NepesInterface::AIInterfaceResultCode NepesInterface::StartTextClassification()
{
	AIInterfaceResultCode return_code = AIInterfaceResultCode::NEPES_SUCCESS;

	//2020-01-03 Tact Time 추가
	clock_t this_time = clock();

	py_gil = PyGILState_Ensure();

	PyObject* input_position = nullptr;
	PyObject* detection_position = nullptr;
	PyObject* image_path = nullptr;
	PyObject* argument_list = nullptr;
	PyObject* classification_result_list = nullptr;

	do {
		if (!g_loaded_ai_model) {
			return_code = AIInterfaceResultCode::NEPES_ERROR_NOT_LOAD_AI_MODEL;
			break;
		}

		input_position = ConvertVectorToListInnerTuple(g_detection_merge_result, return_code);

		if (return_code != AIInterfaceResultCode::NEPES_SUCCESS) {
			break;
		}

		if (!input_position) {
			return_code = AIInterfaceResultCode::NEPES_ERROR_FAILED_TO_CREATE_AI_INPUT_LIST;
			break;
		}

		detection_position = ConvertVectorToDoubleListInnerTuple(g_detection_merge_result, return_code);

		if (return_code != AIInterfaceResultCode::NEPES_SUCCESS) {
			break;
		}

		if (!detection_position) {
			return_code = AIInterfaceResultCode::NEPES_ERROR_FAILED_TO_CREATE_AI_INPUT_LIST;
			break;
		}

		image_path = PyUnicode_FromString(g_image_path.c_str());
		argument_list = Py_BuildValue("(O, O, O, O, O)", g_ai_option, g_classification_model, image_path, input_position, detection_position);

		if (!argument_list) {
			return_code = AIInterfaceResultCode::NEPES_ERROR_FAILED_TO_GET_CLASSIFICATION_RUN_ARGUMENT_LIST;
			break;
		}

		classification_result_list = PythonFunctionCall("classification_run", return_code, argument_list);

		if (!classification_result_list) {
			return_code = AIInterfaceResultCode::NEPES_ERROR_AI_CLASSIFICATION_RESULT_NULL;
			break;
		}

		return_code = ConvertListInnerTupleToVector(classification_result_list, g_text_inference_result);

	} while (false);

	PyGILState_Release(py_gil);

	double this_tact_time = ((double)clock() - this_time);

	std::ofstream output_file("D:\\Result\\Log.txt", std::ios::app);

	output_file << "Lot : " << g_lot_number << " Reel : " << g_reel_total_count << " StartTextClassification() : " << this_tact_time << "ms" << std::endl;

	output_file.close();

	return return_code;
}

NEPES_INTERFACE_API NepesInterface::AIInterfaceResultCode NepesInterface::CompareResultToMES()
{
	AIInterfaceResultCode return_code = AIInterfaceResultCode::NEPES_SUCCESS;

	//2020-01-03 Tact Time 추가
	clock_t this_time = clock();

	do {

		if (!g_loaded_ai_model) {
			return_code = AIInterfaceResultCode::NEPES_ERROR_NOT_LOAD_AI_MODEL;
			break;
		}
		//object 비교
		for (int j = 0; j < g_object_teaching_info.size(); ++j) {
			if (g_object_teaching_info[j].exception) {
				g_object_teaching_info[j].comparison = true;
				continue;
			}

			for (int i = 0; i < g_object_info_merge_result.size(); ++i) {
				if (g_object_info_merge_result[i].current_text_info.object_id == g_object_teaching_info[j].object_id) {
					g_object_teaching_info[j].comparison = true;
				}
			}
		}
		//object 판단
		for (int i = 0; i < g_object_teaching_info.size(); ++i) {
			if (!g_object_teaching_info[i].comparison) {

				NepesInterface::ResultDataset result_dataset;
				result_dataset.mes_data = g_object_teaching_info[i].text_data.c_str();
				result_dataset.result_data = g_object_teaching_info[i].text_data.c_str();
				result_dataset.object_id = g_object_teaching_info[i].object_id;
				result_dataset.type = NepesInterface::DataType::TYPE_OBJECT;
				result_dataset.reel_no = g_object_teaching_info[i].reel_no;
				result_dataset.label_no = g_object_teaching_info[i].label_no;
				result_dataset.pass = false;
				result_dataset.image_name = g_object_teaching_info[i].image_name.c_str();
				result_dataset.error_type = g_ai_result_type[(int)AIResultType::NEPES_AI_RESULT_FAIL_READING].c_str();
				result_dataset.teaching_left_top = g_object_teaching_info[i].bounding_boxes.left_top;
				result_dataset.teaching_right_top = g_object_teaching_info[i].bounding_boxes.right_top;
				result_dataset.teaching_right_bottom = g_object_teaching_info[i].bounding_boxes.right_bottom;
				result_dataset.teaching_left_bottom = g_object_teaching_info[i].bounding_boxes.left_bottom;

				g_object_teaching_info[i].comparison = true;

				g_output_result_data.push_back(result_dataset);

				g_reel_pass &= result_dataset.pass;
			}
		}
		//바코드 비교
		for (int j = 0; j < g_barcode_teaching_info.size(); ++j) {

			if (g_barcode_teaching_info[j].exception) {
				g_barcode_teaching_info[j].comparison = true;
				continue;
			}

			for (int i = 0; i < g_barcode_reading_result.size(); ++i) {
				if (g_barcode_reading_result[i].object_id != g_barcode_teaching_info[j].object_id) {
					continue;
				}

				NepesInterface::ResultDataset result_dataset;
				result_dataset.mes_data = g_barcode_teaching_info[j].barcode_data.c_str();
				result_dataset.result_data = g_barcode_reading_result[i].barcode_data.c_str();
				result_dataset.object_id = g_barcode_teaching_info[j].object_id;
				result_dataset.type = NepesInterface::DataType::TYPE_BARCODE;
				result_dataset.reel_no = g_barcode_teaching_info[j].reel_no;
				result_dataset.label_no = g_barcode_teaching_info[j].label_no;
				result_dataset.image_name = g_barcode_teaching_info[j].image_name.c_str();
				result_dataset.teaching_left_top = g_barcode_teaching_info[j].bounding_boxes.left_top;
				result_dataset.teaching_right_top = g_barcode_teaching_info[j].bounding_boxes.right_top;
				result_dataset.teaching_right_bottom = g_barcode_teaching_info[j].bounding_boxes.right_bottom;
				result_dataset.teaching_left_bottom = g_barcode_teaching_info[j].bounding_boxes.left_bottom;

				if (g_barcode_reading_result[i].barcode_data == g_barcode_teaching_info[j].barcode_data) {
					result_dataset.pass = true;
					result_dataset.error_type = g_ai_result_type[(int)AIResultType::NEPES_AI_RESULT_PASS].c_str();
				}
				else {
					result_dataset.pass = false;
					result_dataset.error_type = g_ai_result_type[(int)AIResultType::NEPES_AI_RESULT_FAIL_MATCHING].c_str();
				}

				g_output_result_data.push_back(result_dataset);

				g_barcode_teaching_info[j].comparison = true;

				g_reel_pass &= result_dataset.pass;
			}
		}
		//텍스트 비교
		for (int j = 0; j < g_current_text_info.size(); ++j) {

			if (g_current_text_info[j].exception) {
				g_current_text_info[j].comparison = true;
				continue;
			}

			for (int i = 0; i < g_text_inference_result.size(); ++i) {
				if (g_text_inference_result[i].object_id != g_current_text_info[j].object_id) {
					continue;
				}

				NepesInterface::ResultDataset result_dataset;
				result_dataset.mes_data = g_current_text_info[j].text_data.c_str();
				result_dataset.object_id = g_current_text_info[j].object_id;
				result_dataset.type = NepesInterface::DataType::TYPE_TEXT;
				result_dataset.reel_no = g_current_text_info[j].reel_no;
				result_dataset.label_no = g_current_text_info[j].label_no;
				result_dataset.image_name = g_current_text_info[j].image_name.c_str();
				result_dataset.teaching_left_top = g_current_text_info[j].bounding_boxes.left_top;
				result_dataset.teaching_right_top = g_current_text_info[j].bounding_boxes.right_top;
				result_dataset.teaching_right_bottom = g_current_text_info[j].bounding_boxes.right_bottom;
				result_dataset.teaching_left_bottom = g_current_text_info[j].bounding_boxes.left_bottom;

				//공백 제거
				std::string mes_text = g_current_text_info[j].text_data;
				std::string inference_text = g_text_inference_result[i].text_data;

				mes_text.erase(std::remove(mes_text.begin(), mes_text.end(), ' '), mes_text.end());
				inference_text.erase(std::remove(inference_text.begin(), inference_text.end(), ' '), inference_text.end());

				if (mes_text == inference_text) {
					result_dataset.pass = true;
					result_dataset.error_type = g_ai_result_type[(int)AIResultType::NEPES_AI_RESULT_PASS].c_str();
					result_dataset.result_data = g_text_inference_result[i].text_data.c_str();
				}

				//임시 예외처리
				else {

					CheckExceptionInformation(i, j , inference_text, mes_text, result_dataset);
				}

				g_output_result_data.push_back(result_dataset);

				g_current_text_info[j].comparison = true;

				g_reel_pass &= result_dataset.pass;
			}
		}

		//추가사항 : 기준 정보랑 데이터 갯수가 맞는지를 확인해야 한다!
		for (int i = 0; i < g_barcode_teaching_info.size(); ++i) {
			if (!g_barcode_teaching_info[i].comparison) {

				NepesInterface::ResultDataset result_dataset;
				result_dataset.mes_data = g_barcode_teaching_info[i].barcode_data.c_str();
				result_dataset.result_data = "";
				result_dataset.object_id = g_barcode_teaching_info[i].object_id;
				result_dataset.type = NepesInterface::DataType::TYPE_BARCODE;
				result_dataset.reel_no = g_barcode_teaching_info[i].reel_no;
				result_dataset.label_no = g_barcode_teaching_info[i].label_no;
				result_dataset.pass = false;
				result_dataset.image_name = g_barcode_teaching_info[i].image_name.c_str();
				result_dataset.error_type = g_ai_result_type[(int)AIResultType::NEPES_AI_RESULT_FAIL_READING].c_str();
				result_dataset.teaching_left_top = g_barcode_teaching_info[i].bounding_boxes.left_top;
				result_dataset.teaching_right_top = g_barcode_teaching_info[i].bounding_boxes.right_top;
				result_dataset.teaching_right_bottom = g_barcode_teaching_info[i].bounding_boxes.right_bottom;
				result_dataset.teaching_left_bottom = g_barcode_teaching_info[i].bounding_boxes.left_bottom;

				g_barcode_teaching_info[i].comparison = true;

				g_output_result_data.push_back(result_dataset);

				g_reel_pass &= result_dataset.pass;
			}
		}

		for (int i = 0; i < g_current_text_info.size(); ++i) {
			if (!g_current_text_info[i].comparison) {

				//예외처리
				//PARTIAL 문구의 경우
				if (g_current_text_info[i].text_data == "") {
					g_current_text_info[i].comparison = true;
					continue;
				}

				NepesInterface::ResultDataset result_dataset;
				result_dataset.mes_data = g_current_text_info[i].text_data.c_str();
				result_dataset.result_data = "";
				result_dataset.object_id = g_current_text_info[i].object_id;
				result_dataset.type = NepesInterface::DataType::TYPE_TEXT;
				result_dataset.reel_no = g_current_text_info[i].reel_no;
				result_dataset.label_no = g_current_text_info[i].label_no;
				result_dataset.pass = false;
				result_dataset.image_name = g_current_text_info[i].image_name.c_str();
				result_dataset.error_type = g_ai_result_type[(int)AIResultType::NEPES_AI_RESULT_FAIL_READING].c_str();
				result_dataset.teaching_left_top = g_current_text_info[i].bounding_boxes.left_top;
				result_dataset.teaching_right_top = g_current_text_info[i].bounding_boxes.right_top;
				result_dataset.teaching_right_bottom = g_current_text_info[i].bounding_boxes.right_bottom;
				result_dataset.teaching_left_bottom = g_current_text_info[i].bounding_boxes.left_bottom;

				g_current_text_info[i].comparison = true;

				g_output_result_data.push_back(result_dataset);

				g_reel_pass &= result_dataset.pass;
			}
		}

		std::sort(g_output_result_data.begin(), g_output_result_data.end(), SortResultObjectID);

	} while (false);

	double this_tact_time = ((double)clock() - this_time);

	std::ofstream output_file("D:\\Result\\Log.txt", std::ios::app);

	output_file << "Lot : " << g_lot_number << " Reel : " << g_reel_total_count << " CompareResultToMES() : " << this_tact_time << "ms" << std::endl;

	output_file.close();
	
	return return_code;
}

NEPES_INTERFACE_API int NepesInterface::GetResultDataSize()
{
	return (int)g_output_result_data.size();
}

NEPES_INTERFACE_API NepesInterface::AIInterfaceResultCode __cdecl NepesInterface::GetResultData(_Out_ ResultDataset result_dataset[])
{
	AIInterfaceResultCode return_code = AIInterfaceResultCode::NEPES_SUCCESS;
	std::ofstream output_file("D:\\Result\\Log.txt", std::ios::app);

	output_file << "GetResultData" << std::endl;
	//2020-01-03 Tact Time 추가
	clock_t this_time = clock();

	bool invalid_index = false;

	do {

		py_gil = PyGILState_Ensure();
		
		if (!g_loaded_ai_model) {
			return_code = AIInterfaceResultCode::NEPES_ERROR_NOT_LOAD_AI_MODEL;
			break;
		}

		PyGILState_Release(py_gil);

		if (!result_dataset) {
			return_code = AIInterfaceResultCode::NEPES_ERROR_INPUT_DATA_EMPTY_OR_NULL;
			break;
		}

		for (int i = 0; i < (int)g_output_result_data.size(); ++i) {
			result_dataset[i].object_id = g_output_result_data[i].object_id;
			result_dataset[i].result_data = g_output_result_data[i].result_data;
			result_dataset[i].mes_data = g_output_result_data[i].mes_data;
			result_dataset[i].pass = g_output_result_data[i].pass;
			result_dataset[i].type = g_output_result_data[i].type;
		}

		SaveResultToFile();

		output_file << "GetResultData End" << std::endl;

	} while (false);

	if (invalid_index) {
		return_code = AIInterfaceResultCode::NEPES_ERROR_RESULT_DATA_INVALID_INDEX;
	}

	double this_tact_time = ((double)clock() - this_time);
	double total_tact_time = ((double)clock() - start_total_time);

	output_file << "Lot : " << g_lot_number << " Reel : " << g_reel_total_count << " GetResultData() : " << this_tact_time << "ms" << std::endl;
	output_file << "End. This Label Tact-Time : " << total_tact_time << "ms" << std::endl;

	output_file.close();

	return return_code;
}

NEPES_INTERFACE_API bool NepesInterface::CheckReelPass()
{
	if (g_reel_pass) {

		++g_reel_total_count;

		//SaveTotalReelCount();

		g_lot_pass &= g_reel_pass;

		return true;
	}
	else {
		return false;
	}
}

NEPES_INTERFACE_API void NepesInterface::AddTotalLotCount()
{
	++g_lot_total_count;

	//SaveTotalLotCount();
}

NEPES_INTERFACE_API bool NepesInterface::WriteComtestData(_In_ const char* lot_number, _In_ int reel_no, _In_ int current_label_index)
{
	bool result = false;

	//libxl::Book* book = nullptr;

	do {
		
		//book =  xlCreateBook();
		std::string lot_number_temp = lot_number;

		if (lot_number_temp != g_before_lot_number) {

			if (!g_book) {
				break;
			}

			g_book->release();

			g_book = xlCreateBook();

			g_before_lot_number = lot_number_temp;
		}

		std::wstring w_lot_no;
		std::string temp_lot_number = lot_number;
		w_lot_no.assign(temp_lot_number.begin(), temp_lot_number.end());

		std::wstring w_reel_no = std::to_wstring(reel_no);
		std::wstring w_label_no = std::to_wstring(current_label_index);

		std::wstring sheet_name = w_lot_no + L"_" + w_reel_no + L"_" + w_label_no;

		
		libxl::Sheet* sheet = g_book->addSheet(sheet_name.c_str());
		if (!sheet) {
			break;
		}

		sheet->setDisplayGridlines(true);

		libxl::Font* title_font = g_book->addFont();
		if (!title_font) {
			break;
		}

		libxl::Font* pass_font = g_book->addFont();
		if (!pass_font) {
			break;
		}

		libxl::Font* fail_font = g_book->addFont();
		if (!fail_font) {
			break;
		}

		libxl::Format* title_format = g_book->addFormat();
		if (!title_format) {
			break;
		}

		libxl::Format* pass_format = g_book->addFormat();
		if (!pass_format) {
			break;
		}

		libxl::Format* fail_format = g_book->addFormat();
		if (!fail_format) {
			break;
		}

		title_font->setSize(14);
		title_font->setBold(true);

		pass_font->setSize(12);
		pass_font->setColor(libxl::COLOR_GREEN);

		fail_font->setSize(12);
		fail_font->setColor(libxl::COLOR_RED);

		title_format->setAlignH(libxl::ALIGNH_LEFT);
		title_format->setAlignV(libxl::ALIGNV_CENTER);

		pass_format->setAlignH(libxl::ALIGNH_LEFT);
		pass_format->setAlignV(libxl::ALIGNV_CENTER);
		pass_format->setFont(pass_font);

		fail_format->setAlignH(libxl::ALIGNH_LEFT);
		fail_format->setAlignV(libxl::ALIGNV_CENTER);
		fail_format->setFont(fail_font);

		std::string image_path = g_image_path;
		std::wstring w_image_path;
		w_image_path.assign(image_path.begin(), image_path.end());

		int id = g_book->addPicture(w_image_path.c_str());
		if (id == -1) {
			break;
		}

		sheet->setMerge(1, 1, 0, 1);
		sheet->setCol(0, 1, 80);
		sheet->setRow(1, 250);

		for (int i = 2; i < 50; ++i) {
			sheet->setRow(i, 15);
		}

		sheet->setPicture(1, 0, id, -1.5, 0, 0, libxl::POSITION_MOVE_AND_SIZE);

		//wide char 형으로 변환해야 함
		struct TempDataset {
			std::wstring inference_data;
			std::wstring mes_data;
			bool pass;
		};

		std::vector<TempDataset> barcode_dataset;
		std::vector<TempDataset> text_dataset;

		for (int i = 0; i < g_output_result_data.size(); ++i) {

			std::string temp_mes_data = g_output_result_data[i].mes_data;
			std::string temp_result_data = g_output_result_data[i].result_data;

			TempDataset temp_dataset;
			temp_dataset.mes_data.assign(temp_mes_data.begin(), temp_mes_data.end());
			temp_dataset.inference_data.assign(temp_result_data.begin(), temp_result_data.end());
			temp_dataset.pass = g_output_result_data[i].pass;

			if (g_output_result_data[i].type == NepesInterface::DataType::TYPE_BARCODE) {
				barcode_dataset.push_back(temp_dataset);
			}
			else if (g_output_result_data[i].type == NepesInterface::DataType::TYPE_TEXT) {
				text_dataset.push_back(temp_dataset);
			}
		}

		int data_start_row = 3;

		sheet->writeStr(2, 0, L"AI Barcode", title_format);
		sheet->writeStr(2, 1, L"MES Barcode", title_format);

		for (int i = 0; i < barcode_dataset.size(); ++i) {
			if (barcode_dataset[i].pass) {
				sheet->writeStr(data_start_row + i, 0, barcode_dataset[i].inference_data.c_str(), pass_format);
			}
			else {
				sheet->writeStr(data_start_row + i, 0, barcode_dataset[i].inference_data.c_str(), fail_format);
			}
			sheet->writeStr(data_start_row + i, 1, barcode_dataset[i].mes_data.c_str(), pass_format);		
		}

		data_start_row = data_start_row + (int)barcode_dataset.size();

		sheet->writeStr(data_start_row, 0, L"AI Text", title_format);
		sheet->writeStr(data_start_row, 1, L"MES Text", title_format);

		++data_start_row;

		for (int i = 0; i < text_dataset.size(); ++i) {
			if (text_dataset[i].pass) {
				sheet->writeStr(data_start_row + i, 0, text_dataset[i].inference_data.c_str(), pass_format);
			}
			else {
				sheet->writeStr(data_start_row + i, 0, text_dataset[i].inference_data.c_str(), fail_format);
			}
			sheet->writeStr(data_start_row + i, 1, text_dataset[i].mes_data.c_str(), pass_format);
		}

		time_t time_var;
		struct tm time_struct;

		time_var = time(NULL);
		localtime_s(&time_struct, &time_var);

		std::wstring year = std::to_wstring(time_struct.tm_year + 1900);
		std::wstring month = std::to_wstring(time_struct.tm_mon + 1);
		std::wstring day = std::to_wstring(time_struct.tm_mday);
		std::wstring hour = std::to_wstring(time_struct.tm_hour);
		std::wstring min = std::to_wstring(time_struct.tm_min);
		std::wstring sec = std::to_wstring(time_struct.tm_sec);

		//std::wstring w_file_name = year + month + day + L"_" + hour + min + sec + L"_" + w_lot_no + L"_" + w_reel_no + L"_" + w_label_no + L".xls";
		std::wstring w_file_name = year + month + day + L"_" + w_lot_no + L".xls";

		std::wstring w_root_path = L"D:\\Comtest";
		std::wstring w_day_path = w_root_path + L"\\" + year + month + day;
		std::wstring w_lot_path = w_day_path + L"\\" + w_lot_no;

		std::wstring w_full_path = w_lot_path + L"\\" + w_file_name;

		_wmkdir(w_root_path.c_str());
		_wmkdir(w_day_path.c_str());
		_wmkdir(w_lot_path.c_str());
		
		result = g_book->save(w_full_path.c_str(), true);

		//g_book->release();

	} while (false);

	return result;
}

NEPES_INTERFACE_API void NepesInterface::ClearLabelData()
{
	g_image_path.clear();

	g_detection_result.clear();

	g_current_text_info.clear();
	g_barcode_teaching_info.clear();
	g_object_teaching_info.clear();

	g_detection_merge_result.clear();

	g_wrapper_barcode_data.clear();

	g_text_inference_result.clear();
	g_barcode_reading_result.clear();
	g_object_info_merge_result.clear();

	g_output_result_data.clear();
	//g_temp_output_result_data.clear();
}

NEPES_INTERFACE_API void NepesInterface::ClearLotData()
{
	g_mes_barcode_information.clear();
	g_mes_text_information.clear();
	g_mes_object_information.clear();

	//g_front_reel_label_number = 0;

	g_reel_pass = true;
	g_lot_pass = true;
}

NEPES_INTERFACE_API NepesInterface::MESResultCode NepesInterface::RegisterOperationStartToMES(_In_ LotInformation lot_info, _Out_ MESResponseSet* response_data)
{
	MESResultCode mes_result = MESResultCode::NEPES_SUCCESS;

	std::wofstream output_file("D:\\Result\\mes_request.txt", std::ios::app);

	do
	{
		std::string request_url = g_mes_if_base_url;

		//LT 버려야 됨
		std::string real_lot_number = lot_info.lot_number;
		size_t find_index = real_lot_number.find("LT");

		if (find_index >= 0) {
			real_lot_number.replace(find_index, 2, "");
		}
		
		//ID 버려야 됨
		std::string operator_id = lot_info.operator_id;
		find_index = operator_id.find("ID");

		if (find_index >= 0) {
			operator_id.replace(find_index, 2, "");
		}

		std::string equipment_name = lot_info.equipment_name;

		request_url = SetUrlParameter(request_url, GetMESParameterName(MESParameterName::NEPES_MES_PARAMETER_NAME_PLANT), GetMESConstParameterValue(MESConstParameterValue::NEPES_MES_PARAMETER_VALUE_PLANT_RUNNING), true);
		request_url = SetUrlParameter(request_url, GetMESParameterName(MESParameterName::NEPES_MES_PARAMETER_NAME_TRANSACTION), GetMESConstParameterValue(MESConstParameterValue::NEPES_MES_PARAMETER_VALUE_TRANSACTION_SPIN), true);
		request_url = SetUrlParameter(request_url, GetMESParameterName(MESParameterName::NEPES_MES_PARAMETER_NAME_LOT_NUMBER), real_lot_number, true);
		request_url = SetUrlParameter(request_url, GetMESParameterName(MESParameterName::NEPES_MES_PARAMETER_NAME_USER), operator_id, true);
		request_url = SetUrlParameter(request_url, GetMESParameterName(MESParameterName::NEPES_MES_PARAMETER_NAME_EQUIPMENT), equipment_name, false);

		std::wstring w_request_url;
		w_request_url.assign(request_url.begin(), request_url.end());

		output_file << L"RegisterOperationStartToMES" << std::endl;
		output_file << w_request_url << std::endl;

		web::json::value json_result = GetJson(w_request_url);
		if (json_result.is_null()) {
			mes_result = MESResultCode::NEPES_ERROR_FAILED_TO_REQUEST_REGISTER_OPERATION_START;
			break;
		}

		ClearWrapperMESResponseSet();

		web::json::value json_status = json_result.at(GetMESCommonKey(MESCommonKey::NEPES_MES_KEY_STATUS));
		if (json_status.is_string()) {

			std::string result = WStringToString(json_status.as_string().c_str());

			if (result == "ERROR") {
				mes_result = MESResultCode::NEPES_ERROR_MES_RETURN_STATUS_ERROR;
			}

			g_mes_wrapper_response_set.status = result;
			response_data->status = g_mes_wrapper_response_set.status.c_str();
		}

		/*web::json::value json_records = json_result.at(GetMESCommonKey(MESCommonKey::NEPES_MES_KEY_RECORDS));*/
		web::json::value json_records = json_result.at(GetMESCommonKey(MESCommonKey::NEPES_MES_KEY_MESSAGE));
		if (json_records.is_string()) {

			std::string result = WStringToString(json_records.as_string().c_str());

			g_mes_wrapper_response_set.result = result;
			response_data->result = g_mes_wrapper_response_set.result.c_str();
		}

	} while (false);

	output_file.close();

	return mes_result;
}

NEPES_INTERFACE_API NepesInterface::MESResultCode NepesInterface::GetRecipeTypeFromMES(_In_ LotInformation lot_info, _Out_ MESResponseSet* response_data, _Out_ ModelInformation* model_information)
{
	MESResultCode mes_result = MESResultCode::NEPES_SUCCESS;

	std::wofstream output_file("D:\\Result\\mes_request.txt", std::ios::app);

	do
	{
		std::string request_url = g_ai_if_base_url;

		//LT 버려야 됨
		std::string real_lot_number = lot_info.lot_number;
		size_t find_index = real_lot_number.find("LT");

		if (find_index >= 0) {
			real_lot_number.replace(find_index, 2, "");
		}

		//ID 버려야 됨
		std::string operator_id = lot_info.operator_id;
		find_index = operator_id.find("ID");

		if (find_index >= 0) {
			operator_id.replace(find_index, 2, "");
		}

		std::string equipment_id = lot_info.equipment_name;

		request_url = SetUrlParameter(request_url, GetMESParameterName(MESParameterName::NEPES_MES_PARAMETER_NAME_PLANT), GetMESConstParameterValue(MESConstParameterValue::NEPES_MES_PARAMETER_VALUE_PLANT_RUNNING), true);
		request_url = SetUrlParameter(request_url, GetMESParameterName(MESParameterName::NEPES_MES_PARAMETER_NAME_ACTION_TYPE), GetMESConstParameterValue(MESConstParameterValue::NEPES_MES_PARAMETER_VALUE_ACTION_TYPE_GET_RECIPE_TYPE), true);
		request_url = SetUrlParameter(request_url, GetMESParameterName(MESParameterName::NEPES_MES_PARAMETER_NAME_LOT_NUMBER), real_lot_number, true);
		request_url = SetUrlParameter(request_url, GetMESParameterName(MESParameterName::NEPES_MES_PARAMETER_NAME_USER), operator_id, true);
		request_url = SetUrlParameter(request_url, GetMESParameterName(MESParameterName::NEPES_MES_PARAMETER_NAME_EQUIPMENT), equipment_id, false);

		std::wstring w_request_url;
		w_request_url.assign(request_url.begin(), request_url.end());

		output_file << L"GetRecipeTypeFromMES" << std::endl;
		output_file << w_request_url << std::endl;

		web::json::value json_result = GetJson(w_request_url);

		if (json_result.is_null()) {
			mes_result = MESResultCode::NEPES_ERROR_FAILED_TO_REQUEST_GET_RECIPE_TYPE;
			break;
		}

		ClearWrapperMESResponseSet();

		web::json::value json_status = json_result.at(GetMESCommonKey(MESCommonKey::NEPES_MES_KEY_STATUS));
		if (json_status.is_string()) {

			std::string result = WStringToString(json_status.as_string().c_str());

			if (result == "ERROR") {
				mes_result = MESResultCode::NEPES_ERROR_MES_RETURN_STATUS_ERROR;
			}

			g_mes_wrapper_response_set.status = result;
			response_data->status = g_mes_wrapper_response_set.status.c_str();
		}
		web::json::value json_records = json_result.at(GetMESCommonKey(MESCommonKey::NEPES_MES_KEY_RECORDS));

		if (json_records.is_object()) {

			ClearWrapperModelInformation();
			ConvertRecipeTypeJsonToStruct(json_records, g_wrapper_model_information);

			model_information->type = g_wrapper_model_information.type;
			model_information->customer = g_wrapper_model_information.customer.c_str();
			model_information->recipe = g_wrapper_model_information.recipe.c_str();
		}
		else if (json_records.is_string()) {

			std::string result = WStringToString(json_records.as_string().c_str());

			g_mes_wrapper_response_set.result = result;
			response_data->result = g_mes_wrapper_response_set.result.c_str();
		}

	} while (false);

	output_file.close();

	return mes_result;
}

NEPES_INTERFACE_API NepesInterface::MESResultCode NepesInterface::GetReelInformationFromMES(_In_ LotInformation lot_info, _Out_ MESResponseSet* response_data, _Out_ int* reel_number)
{
	MESResultCode mes_result = MESResultCode::NEPES_SUCCESS;

	std::wofstream output_file("D:\\Result\\mes_request.txt", std::ios::app);

	do
	{
		std::string request_url = g_ai_if_base_url;

		//LT 버려야 됨
		std::string real_lot_number = lot_info.lot_number;
		size_t find_index = real_lot_number.find("LT");

		if (find_index >= 0) {
			real_lot_number.replace(find_index, 2, "");
		}

		g_lot_number = real_lot_number;

		//ID 버려야 됨
		std::string operator_id = lot_info.operator_id;
		find_index = operator_id.find("ID");

		if (find_index >= 0) {
			operator_id.replace(find_index, 2, "");
		}

		std::string equipment_name = lot_info.equipment_name;

		request_url = SetUrlParameter(request_url, GetMESParameterName(MESParameterName::NEPES_MES_PARAMETER_NAME_PLANT), GetMESConstParameterValue(MESConstParameterValue::NEPES_MES_PARAMETER_VALUE_PLANT_RUNNING), true);
		request_url = SetUrlParameter(request_url, GetMESParameterName(MESParameterName::NEPES_MES_PARAMETER_NAME_ACTION_TYPE), GetMESConstParameterValue(MESConstParameterValue::NEPES_MES_PARAMETER_VALUE_ACTION_TYPE_GET_PISP_DATA), true);
		request_url = SetUrlParameter(request_url, GetMESParameterName(MESParameterName::NEPES_MES_PARAMETER_NAME_LOT_NUMBER), real_lot_number, true);
		request_url = SetUrlParameter(request_url, GetMESParameterName(MESParameterName::NEPES_MES_PARAMETER_NAME_USER), operator_id, true);
		//request_url = SetUrlParameter(request_url, GetMESParameterName(MESParameterName::NEPES_MES_PARAMETER_NAME_USER), GetMESConstParameterValue(MESConstParameterValue::NEPES_MES_PARAMETER_VALUE_USER), true);
		request_url = SetUrlParameter(request_url, GetMESParameterName(MESParameterName::NEPES_MES_PARAMETER_NAME_EQUIPMENT), equipment_name, false);

		std::wstring w_request_url;
		w_request_url.assign(request_url.begin(), request_url.end());

		output_file << L"GetReelInformationFromMES" << std::endl;
		output_file << w_request_url << std::endl;

		web::json::value json_result = GetJson(w_request_url);

		if (json_result.is_null()) {
			mes_result = MESResultCode::NEPES_ERROR_FAILED_TO_REQUEST_GET_LABEL_INFORMATION;
			break;
		}

		ClearWrapperMESResponseSet();

		web::json::value json_status = json_result.at(GetMESCommonKey(MESCommonKey::NEPES_MES_KEY_STATUS));
		if (json_status.is_string()) {

			std::string result = WStringToString(json_status.as_string().c_str());

			if (result == "ERROR") {
				mes_result = MESResultCode::NEPES_ERROR_MES_RETURN_STATUS_ERROR;
			}

			g_mes_wrapper_response_set.status = result;
			response_data->status = g_mes_wrapper_response_set.status.c_str();
		}

		web::json::value json_records = json_result.at(GetMESCommonKey(MESCommonKey::NEPES_MES_KEY_RECORDS));
		if (json_records.is_array()) {

			ClearLabelInformation();
			ConvertReelInformationJsonToVector(json_records);
			
			int last_index = (int)g_mes_text_information.size() - 1;
			if (last_index < 0) {
				mes_result = MESResultCode::NEPES_ERROR_MES_RETURN_UNVALID_VALUE;
				break;
			}

			*reel_number = g_mes_text_information[last_index].reel_number;
		}
		else if(json_records.is_string()){

			std::string result = WStringToString(json_records.as_string().c_str());

			g_mes_wrapper_response_set.result = result;
			response_data->result = g_mes_wrapper_response_set.result.c_str();
		}

	} while (false);

	output_file.close();

	return mes_result;
}

NEPES_INTERFACE_API NepesInterface::MESResultCode NepesInterface::SendInferenceResultToMES(_In_ LotInformation lot_info, _Out_ MESResponseSet* response_data)
{
	MESResultCode mes_result = MESResultCode::NEPES_SUCCESS;

	std::wofstream output_file("D:\\Result\\mes_request.txt", std::ios::app);

	do
	{
		std::string request_url = g_ai_if_base_url;
		std::string inference_result_data;

		//LT 버려야 됨
		std::string real_lot_number = lot_info.lot_number;
		size_t find_index = real_lot_number.find("LT");

		if (find_index >= 0) {
			real_lot_number.replace(find_index, 2, "");
		}

		//ID 버려야 됨
		std::string operator_id = lot_info.operator_id;
		find_index = operator_id.find("ID");

		if (find_index >= 0) {
			operator_id.replace(find_index, 2, "");
		}

		std::string equipment_name = lot_info.equipment_name;

		//if (g_lot_pass) {
			inference_result_data = "SUCCESS";
		//}
		//else {
		//	inference_result_data = "FAIL";
		//}

		request_url = SetUrlParameter(request_url, GetMESParameterName(MESParameterName::NEPES_MES_PARAMETER_NAME_PLANT), GetMESConstParameterValue(MESConstParameterValue::NEPES_MES_PARAMETER_VALUE_PLANT_RUNNING), true);
		request_url = SetUrlParameter(request_url, GetMESParameterName(MESParameterName::NEPES_MES_PARAMETER_NAME_ACTION_TYPE), GetMESConstParameterValue(MESConstParameterValue::NEPES_MES_PARAMETER_VALUE_ACTION_TYPE_SAVE_INFERENCE_RESULT), true);
		request_url = SetUrlParameter(request_url, GetMESParameterName(MESParameterName::NEPES_MES_PARAMETER_NAME_LOT_NUMBER), real_lot_number, true);
		request_url = SetUrlParameter(request_url, GetMESParameterName(MESParameterName::NEPES_MES_PARAMETER_NAME_USER), operator_id, true);
		//request_url = SetUrlParameter(request_url, GetMESParameterName(MESParameterName::NEPES_MES_PARAMETER_NAME_USER), GetMESConstParameterValue(MESConstParameterValue::NEPES_MES_PARAMETER_VALUE_USER), true);
		request_url = SetUrlParameter(request_url, GetMESParameterName(MESParameterName::NEPES_MES_PARAMETER_NAME_EQUIPMENT), equipment_name, true);
		request_url = SetUrlParameter(request_url, GetMESParameterName(MESParameterName::NEPES_MES_PARAMETER_NAME_RESULT), inference_result_data, false);

		std::wstring w_request_url;
		w_request_url.assign(request_url.begin(), request_url.end());

		output_file << L"SendInferenceResultToMES" << std::endl;
		output_file << w_request_url << std::endl;

		web::json::value json_result = GetJson(w_request_url);

		if (json_result.is_null()) {
			mes_result = MESResultCode::NEPES_ERROR_FAILED_TO_REQUEST_REGISTER_OPERATION_START;
			break;
		}

		ClearWrapperMESResponseSet();

		web::json::value json_status = json_result.at(GetMESCommonKey(MESCommonKey::NEPES_MES_KEY_STATUS));
		if (json_status.is_string()) {

			std::string result = WStringToString(json_status.as_string().c_str());

			if (result == "ERROR") {
				mes_result = MESResultCode::NEPES_ERROR_MES_RETURN_STATUS_ERROR;
			}

			g_mes_wrapper_response_set.status = result;
			response_data->status = g_mes_wrapper_response_set.status.c_str();
		}

		web::json::value json_records = json_result.at(GetMESCommonKey(MESCommonKey::NEPES_MES_KEY_RECORDS));
		if (json_records.is_string()) {

			std::string result = WStringToString(json_records.as_string().c_str());

			g_mes_wrapper_response_set.result = result;
			response_data->result = g_mes_wrapper_response_set.result.c_str();
		}

	} while (false);

	output_file.close();

	return mes_result;
}

NEPES_INTERFACE_API NepesInterface::MESResultCode NepesInterface::RegisterOperationEndToMES(_In_ LotInformation lot_info, _Out_ MESResponseSet* response_data)
{
	MESResultCode mes_result = MESResultCode::NEPES_SUCCESS;

	std::wofstream output_file("D:\\Result\\mes_request.txt", std::ios::app);

	do
	{
		std::string request_url = g_mes_if_base_url;

		//LT 버려야 됨
		std::string real_lot_number = lot_info.lot_number;
		size_t find_index = real_lot_number.find("LT");

		if (find_index >= 0) {
			real_lot_number.replace(find_index, 2, "");
		}

		//ID 버려야 됨
		std::string operator_id = lot_info.operator_id;
		find_index = operator_id.find("ID");

		if (find_index >= 0) {
			operator_id.replace(find_index, 2, "");
		}

		std::string equipment_name = lot_info.equipment_name;

		request_url = SetUrlParameter(request_url, GetMESParameterName(MESParameterName::NEPES_MES_PARAMETER_NAME_PLANT), GetMESConstParameterValue(MESConstParameterValue::NEPES_MES_PARAMETER_VALUE_PLANT_RUNNING), true);
		request_url = SetUrlParameter(request_url, GetMESParameterName(MESParameterName::NEPES_MES_PARAMETER_NAME_TRANSACTION), GetMESConstParameterValue(MESConstParameterValue::NEPES_MES_PARAMETER_VALUE_TRANSACTION_SPOU), true);
		request_url = SetUrlParameter(request_url, GetMESParameterName(MESParameterName::NEPES_MES_PARAMETER_NAME_LOT_NUMBER), real_lot_number, true);
		request_url = SetUrlParameter(request_url, GetMESParameterName(MESParameterName::NEPES_MES_PARAMETER_NAME_USER), operator_id, true);
		//request_url = SetUrlParameter(request_url, GetMESParameterName(MESParameterName::NEPES_MES_PARAMETER_NAME_USER), GetMESConstParameterValue(MESConstParameterValue::NEPES_MES_PARAMETER_VALUE_USER), true);
		request_url = SetUrlParameter(request_url, GetMESParameterName(MESParameterName::NEPES_MES_PARAMETER_NAME_EQUIPMENT), equipment_name, true);
		request_url = SetUrlParameter(request_url, GetMESParameterName(MESParameterName::NEPES_MES_PARAMETER_NAME_OPERATION), GetMESConstParameterValue(MESConstParameterValue::NEPES_MES_PARAMETER_VALUE_OPERATION), false);

		std::wstring w_request_url;
		w_request_url.assign(request_url.begin(), request_url.end());

		output_file << L"RegisterOperationEndToMES" << std::endl;
		output_file << w_request_url << std::endl;

		web::json::value json_result = GetJson(w_request_url);

		if (json_result.is_null()) {
			mes_result = MESResultCode::NEPES_ERROR_FAILED_TO_REQUEST_REGISTER_OPERATION_START;
			break;
		}

		ClearWrapperMESResponseSet();

		web::json::value json_status = json_result.at(GetMESCommonKey(MESCommonKey::NEPES_MES_KEY_STATUS));
		if (json_status.is_string()) {

			std::string result = WStringToString(json_status.as_string().c_str());

			if (result == "ERROR") {
				mes_result = MESResultCode::NEPES_ERROR_MES_RETURN_STATUS_ERROR;
			}

			g_mes_wrapper_response_set.status = result;
			response_data->status = g_mes_wrapper_response_set.status.c_str();
		}

		//web::json::value json_records = json_result.at(GetMESCommonKey(MESCommonKey::NEPES_MES_KEY_RECORDS));
		web::json::value json_records = json_result.at(GetMESCommonKey(MESCommonKey::NEPES_MES_KEY_MESSAGE));
		if (json_records.is_string()) {

			std::string result = WStringToString(json_records.as_string().c_str());

			g_mes_wrapper_response_set.result = result;
			response_data->result = g_mes_wrapper_response_set.result.c_str();
		}

	} while (false);

	output_file.close();

	return mes_result;
}