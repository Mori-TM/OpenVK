#define OpenVkBool uint8_t
#define OpenVkTrue 1
#define OpenVkFalse 0

#ifdef __cplusplus
extern "C" {
#endif
	OPENVK_LIB void OpenVkRuntimeInfo(const char* Msg, const char* Val);

	OPENVK_LIB OpenVkBool OpenVkRuntimeError(const char* Msg);

	OPENVK_LIB void* OpenVkSaveMalloc(size_t Size);

	OPENVK_LIB void* OpenVkSaveRealloc(void* Data, size_t Size);

	OPENVK_LIB char* OpenVkReadFileData(const char* Path, size_t* Size);
#ifdef __cplusplus
}
#endif