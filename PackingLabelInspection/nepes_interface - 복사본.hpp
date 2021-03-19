/* nepes corp. 
   JHLEE 2020-06-29 Start
*/
#pragma once

#include <sal.h>
#include <string>
#include <vector>
#include <atlconv.h>

#define NEPES_INTERFACE_API extern "C" __declspec(dllexport)

namespace NepesInterface
{
	//Return된 enum 값을 C#에서 ToString 함수로 호출해서 Dialog에 출력해주시면 됩니다.  JHLEE
	enum class AIInterfaceResultCode {
		NEPES_SUCCESS,

		NEPES_ERROR_ALREADY_LOAD_AI_MODEL,
		NEPES_ERROR_NOT_LOAD_AI_MODEL,

		NEPES_ERROR_INPUT_DATA_EMPTY_OR_NULL,
		NEPES_ERROR_INVALID_INPUT_SIZE,

		NEPES_ERROR_IMAGE_PATH_EMPTY,
		NEPES_ERROR_FAILED_TO_FIND_PYTHON_FILE,
		NEPES_ERROR_FAILED_TO_FIND_PYTHON_FUNCTION,
		NEPES_ERROR_FAILED_TO_CALLABLE_PYTHON_FUNCTION,

		NEPES_ERROR_FAILED_TO_SET_AI_OPTION,
		NEPES_ERROR_AI_OPTION_OBJECT_NULL,
		NEPES_ERROR_FAILED_TO_CHECK_AI_OPTION,

		NEPES_ERROR_FAILED_TO_GET_DETECTION_MODEL_ARGUMENT_LIST,
		NEPES_ERROR_AI_DETECTION_MODEL_NULL,

		NEPES_ERROR_FAILED_TO_GET_CLASSIFICATION_MODEL_ARGUMENT_LIST,
		NEPES_ERROR_AI_CLASSIFICATION_MODEL_NULL,

		NEPES_ERROR_FAILED_TO_GET_DETECTION_RUN_ARGUMENT_LIST,
		NEPES_ERROR_AI_DETECTION_RESULT_NULL,
		NEPES_ERROR_AI_DETECTION_RESULT_TYPE_NOT_LIST,
		NEPES_ERROR_AI_DETECTION_RESULT_TYPE_NOT_TUPLE,

		NEPES_ERROR_AI_DETECTION_RESULT_SIZE_ZERO,
		NEPES_ERROR_FAILED_TO_GET_TEXT_TEACHING_DATA,
		NEPES_ERROR_FAILED_TO_GET_BARCODE_TEACHING_DATA,
		NEPES_ERROR_FAILED_TO_GET_OBJECT_TEACHING_DATA,
		NEPES_ERROR_FAILED_TO_ALIGN_BARCODE_DATA,

		NEPES_ERROR_FAILED_TO_CREATE_AI_INPUT_LIST,
		NEPES_ERROR_FAILED_TO_GET_CLASSIFICATION_RUN_ARGUMENT_LIST,
		NEPES_ERROR_AI_CLASSIFICATION_RESULT_NULL,
		NEPES_ERROR_AI_CLASSIFICATION_RESULT_TYPE_NOT_LIST,
		NEPES_ERROR_AI_CLASSIFICATION_RESULT_TYPE_NOT_TUPLE,

		NEPES_ERROR_RESULT_DATA_INVALID_INDEX,
	};

	enum class VisionResultCode {
		NEPES_SUCCESS,

		NEPES_ERROR_INVALID_BARCODE_ARRAY_SIZE,
		NEPES_ERROR_BARCODE_ARRAY_POINTER_NULL,
		NEPES_ERROR_INVALID_ROI_ARRAY_SIZE,
		NEPES_ERROR_ROI_ARRAY_POINTER_NULL,
		NEPES_ERROR_INVALID_ROI_NAME,

		NEPES_ERROR_LABEL_EXCEEDED_MINI_LABEL,
		NEPES_ERROR_LABEL_OMITTED_MINI_LABEL,
		NEPES_ERROR_LABEL_EXCEEDED,
		NEPES_ERROR_LABEL_OMITTED,

		NEPES_RESULT_CODE_TOTAL,
	};

	//2021-01-28 JHLEE
	//MES는 에러메시지를 MES에서 직접 주기 때문에 따로 출력할 필요 없습니다.
	enum class MESResultCode {
		NEPES_SUCCESS,
		NEPES_ERROR_MES_RETURN_STATUS_ERROR,
		NEPES_ERROR_MES_RETURN_UNVALID_VALUE,

		NEPES_ERROR_FAILED_TO_REQUEST_REGISTER_OPERATION_START,
		NEPES_ERROR_FAILED_TO_REQUEST_GET_RECIPE_TYPE,
		NEPES_ERROR_FAILED_TO_REQUEST_GET_LABEL_INFORMATION,
	};

	enum class DataType {
		TYPE_BARCODE,
		TYPE_TEXT,
		TYPE_OBJECT,
	};

	enum class ModelType {
		TYPE_ONE_SIDE,
		TYPE_DOUBLE_SIDE,
	};

	enum class ReelPosition {
		POS_FRONT,
		POS_BACK,
	};

	struct Vertex {
		int x;
		int y;
	};

	struct RectanglePosition {
		Vertex left_top;
		Vertex right_top;
		Vertex right_bottom;
		Vertex left_bottom;
	};
	
	struct ResultDataset {
		DataType type;	// Text, Barcode
		int reel_no;  // Reel Number
		int label_no; // Label Number
		int object_id;	// MES Information ID
		const char* result_data;	// Barcode Reading Result or AI Text Inference Result
		const char* mes_data;	// Registered MES Data
		bool pass;
		const char* image_name; // Label Localization Result Image Path
		const char* error_type; // Error Type
		NepesInterface::Vertex teaching_left_top;	//MES Teaching Box Left-Top Position
		NepesInterface::Vertex teaching_right_top;	//MES Teaching Box  Right-Top Position
		NepesInterface::Vertex teaching_right_bottom;	//MES Teaching Box  Right-Bottom Position
		NepesInterface::Vertex teaching_left_bottom;	//MES Teaching Box  Left-Bottom Position
	};

	struct MESResponseSet {
		const char* status;	// Success or Error
		const char* result;	// MES에서 직접 리턴해주는 에러메시지
	};

	struct ModelInformation {
		const char* customer;	//고객사
		const char* recipe;	//Recipe Type
		ModelType type;
	};

	struct BarcodeDataset {
		Vertex center_position;	//리딩한 바코드 사각형 영역의 중점
		const char* barcode_reading_data;	//바코드 리딩 결과
	};

	struct VisionROIInformation {
		RectanglePosition rect_pos;	//Vision에서 설정한 ROI 좌표
		const char* roi_name;	//Vision에서 설정한 ROI 이름
	};

	struct LotInformation {
		const char* lot_number;	//Lot Card에 있는 Lot Number 바코드 값
		const char* operator_id;	//현장 작업자들이 가지고 있는 고유의 사번 값 (품질부서 작업자들만 해당 시스템 접근에 유효함)
		const char* equipment_name;	//설비 명
	};

	struct Message {
		const char* error_message;	//시험 삼아 만든 에러 메시지. 추후 횡전개 시 에러메시지를 전부 nepes에서 관리할 수도 있습니다.
	};

	//주의 : SAL _Out_에 명시된 [] 연산은 C#에서는 1차원 구조체 배열로 받습니다. (마샬링 시 주의)
	//주의 2 : 포인터 변수들은 C#에서는 ref 키워드로 받아야 합니다. (마샬링 시 주의)

	NEPES_INTERFACE_API void StartAIInterfaceModule();
	//'AI Interface Functons' 아래 항목의 함수들을 호출 하기 전에 반드시 호출해야 하는 함수입니다.
	// 최초 한 번만 호출해야 합니다.
	NEPES_INTERFACE_API void StopAIInterfaceModule();
	// 프로그램 종료 전에 호출해야 하는 함수입니다.
	// 호출하지 않을 경우 Memory Leak이 발생합니다.

	//////////////////////////// AI Interface Functions ////////////////////////////
	NEPES_INTERFACE_API AIInterfaceResultCode LoadAIModel();
	// AI Detection, Classification Model을 Load하는 함수입니다.
	// 최초 한 번만 호출해야 합니다.

	NEPES_INTERFACE_API VisionResultCode CheckLabelNumber(_In_ ReelPosition pos, _In_ int reel_no, _In_ int barcode_array_size, _In_ BarcodeDataset barcode_dataset[], _In_ int roi_array_size, _In_ VisionROIInformation roi_info[], _In_ ModelInformation* model_information, _Out_ Message* message);
	//현재 Live 화면에서 읽은 모든 바코드 정보와 ROI 정보들을 확인해서 라벨의 미부착, 초과 부착 여부를 판단합니다.
	//NEPES_SUCCESS 이외의 값이 리턴되면, Lot-Card와 작업자 사번을 읽는 스텝으로 돌아가야 합니다.

	NEPES_INTERFACE_API AIInterfaceResultCode SetImagePath(_In_ const char* image_path);
	//이미지 경로를 설정합니다.
	//배열이 아닌 변수입니다. 문자열 1개 값만 넣습니다. 즉 로컬라이제이션된 하나의 라벨의 이미지 전체 경로를 의미합니다.

	NEPES_INTERFACE_API AIInterfaceResultCode StartTextDetection();
	//nepes AI에 라벨 내 텍스트 디텍션을 수행할 것을 요청합니다.

	NEPES_INTERFACE_API VisionResultCode SetBarcodeReadingResult(_In_ ReelPosition pos, _In_ int array_size, _In_ BarcodeDataset barcode_dataset[]);
	//바코드 리딩 결과를 저장합니다.

	NEPES_INTERFACE_API AIInterfaceResultCode AlignLabelDataByMESInformation(_In_ ReelPosition pos, _In_ int reel_no, _In_ int current_label_index);
	//AI가 Detection한 결과를 MES의 티칭 정보를 기반으로 Merge 합니다.

	NEPES_INTERFACE_API AIInterfaceResultCode StartTextClassification();
	//AI에게 글자 인식을 수행할 것을 요청합니다.
	//AlignLabelDataByMESInformation 함수를 호출한 이후에 호출해야 합니다.

	NEPES_INTERFACE_API AIInterfaceResultCode CompareResultToMES();
	//MES 정보들과 AI가 인식한 결과들의 비교를 수행하도록 요청합니다.

	NEPES_INTERFACE_API int GetResultDataSize();
	//Result Data Size를 반환합니다.
	//리턴되는 값은 다음 GetResultData 함수의 결과를 받을 1차원 구조체 배열의 크기입니다.

	NEPES_INTERFACE_API AIInterfaceResultCode GetResultData(_Out_ ResultDataset result_dataset[]);
	//MES 데이터와 AI 인식결과를 반환합니다.
	
	NEPES_INTERFACE_API bool CheckReelPass();
	//Reel Pass 여부를 확인합니다.

	NEPES_INTERFACE_API void AddTotalLotCount();
	//검사 완료한 Lot의 갯수를 추가 합니다. (Lot Count 관리를 위함)

	NEPES_INTERFACE_API bool WriteComtestData(_In_ const char*  lot_number, _In_ int reel_no, _In_ int current_label_index);
	//Comtest모드용 함수 입니다.

	NEPES_INTERFACE_API void ClearLabelData();
	//1개의 라벨 검사에 필요한 전역 변수들의 값을 초기화 합니다.

	NEPES_INTERFACE_API void ClearLotData();
	//1Lot이 끝나면 내부 데이터를 초기화합니다.

	//////////////////////////// MES Interface Functions ////////////////////////////

	NEPES_INTERFACE_API MESResultCode RegisterOperationStartToMES(_In_ LotInformation lot_info, _Out_ MESResponseSet* response_data);
	//MES에 작업 시작을 등록합니다.
	//Lot 단위로 등록합니다.

	NEPES_INTERFACE_API MESResultCode GetRecipeTypeFromMES(_In_ LotInformation lot_info, _Out_ MESResponseSet* response_data, _Out_ ModelInformation* model_information);
	//MES로 부터 현재 Lot에 해당하는 고객사, 타입, 단면-양면 유무를 얻어옵니다.

	NEPES_INTERFACE_API MESResultCode GetReelInformationFromMES(_In_ LotInformation lot_info, _Out_ MESResponseSet* response_data, _Out_ int* reel_number);
	//MES로 부터 현재 Lot에 해당하는 모든 Reel의 라벨 정보를 얻어옵니다.
	//현재 Lot의 reel 갯수는 reel_number로 반환됩니다.

	NEPES_INTERFACE_API MESResultCode SendInferenceResultToMES(_In_ LotInformation lot_info, _Out_ MESResponseSet* response_data);
	//MES에 검사 결과를 등록합니다.
	//Lot 단위로 등록합니다. (Reel 단위가 아닙니다.)

	NEPES_INTERFACE_API MESResultCode RegisterOperationEndToMES(_In_ LotInformation lot_info, _Out_ MESResponseSet* response_data);
	//MES에 작업 완료를 등록합니다.
	//Lot 단위로 등록합니다.

	///////////////////////////////////////////////////////////////////////////////

	// To.(주)바심 SW Engineer 담당자님
	// 각 모드에 대해 하기와 같은 순서로 호출하시기 바랍니다.
	// PackingLabelInspection.dll은 실행파일이 있는 폴더에 넣으시면 됩니다.
	// From. nepes JHLEE

	//작업 초기화 버튼 클릭 시 아래 함수 호출
	// ClearLabelData() -> ClearLotData()

	//프로그램 시작 후
	//StartAIInterfaceModule() -> LoadAIModel()

	//프로그램 종료 시점
	//StopAIInterfaceModule()

	/////////// Running Mode ///////////

	//단면 래시피
	//Lot Cycle
	//Lot Card와 사번 Loading-> 첫 번째 그랩 -> Lot Number와 사번 리딩-> RegisterOperationStartToMES()-> GetRecipeTypeFromMES()-> Recipe Load 되면 ->GetReelInformationFromMES()

		//Reel Cycle Start(Lot에 있는 Reel 개수만큼 반복)
		//ROI에 맞춰서 제품 로딩 후 두 번째 그랩 -> CheckLabelNumber()

			//Label Cycle(Reel에 있는 Label 개수만큼 반복)
			//Label Localization -> Barcode Reading -> SetImagePath() -> StartTextDetection() -> SetBarcodeReadingResult() -> AlignLabelDataByMESInformation() 
			//-> StartTextClassification() -> CompareResultToMES()-> GetResultDataSize() -> GetResultData() -> ClearLabelData() -> Label Localization 스텝으로 이동해서 라벨 개수만큼 반복

			//Label Cycle End
			//모든 라벨 검사 끝났으면 -> CheckReelPass() -> ROI에 맞춰서 제품 로딩 스텝으로 이동

		//Reel Cycle End
		//ClearLotData() -> AddTotalLotCount() -> SendInferenceResultToMES() -> RegisterOperationEndToMES() -> Lot Card와 사번 Loading 스텝으로 복귀

	//Lot Cycle End

	//양면 래시피
	//Lot Cycle
	//Lot Card와 사번 Loading -> 첫 번째 그랩 -> Lot Number와 사번 리딩 -> RegisterOperationStartToMES() -> GetRecipeTypeFromMES() ? > Recipe_F에 해당하는 Recipe Load -> GetReelInformationFromMES()
	
		//Reel Cycle Start(Lot에 있는 Reel 개수만큼 반복)
		//ROI에 맞춰서 제품 로딩 후 두 번째 그랩 -> CheckLabelNumber()

			//Label Cycle(Reel에 있는 Label 개수만큼 반복)
			//Label Localization -> Barcode Reading -> SetImagePath() -> StartTextDetection() -> SetBarcodeReadingResult() -> AlignLabelDataByMESInformation() 
			//-> StartTextClassification() -> CompareResultToMES() -> GetResultDataSize() -> GetResultData() -> ClearLabelData() -> Label Localization 스텝으로 이동해서 라벨 개수만큼 반복
			
			//Label Cycle End
			//1. 앞, 뒷면 모든 라벨 검사 끝났으면 -> CheckReelPass() -> ROI에 맞춰서 제품 로딩 스텝으로 이동
			//2. 뒷면이 아직 안끝났으면 -> Recipe_B 에 해당하는 Recipe Load -> ROI에 맞춰서 제품 로딩 후 두 번째 그랩 스텝으로 이동

		//Reel Cycle End
		//ClearLotData()->AddTotalLotCount()->SendInferenceResultToMES()->RegisterOperationEndToMES()->Lot Card와 사번 Loading 스텝으로 복귀
	//	Lot Cycle End



}









