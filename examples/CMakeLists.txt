PROJECT(Image)

add_executable(white_noise white_noise.cpp)
target_link_libraries(white_noise
	image
	jpeg
)
add_executable(primitive primitive.cpp)
target_link_libraries(primitive
	image
)
