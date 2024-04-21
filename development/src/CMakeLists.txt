file(GLOB DLMS_SRC
     "*.c"
	 "*.cpp"
)

add_library(gurux_dlms_cpp ${DLMS_SRC})