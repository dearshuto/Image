PROJECT(FUJI-Ray)

add_executable(white_noise white_noise.cpp)
target_link_libraries(white_noise
	image
)