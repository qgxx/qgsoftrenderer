add_library(renderer 
platform/win_app.cpp 
textures/texture.cpp 
frame_buffer.cpp 
model.cpp 
pipeline.cpp 
renderer.cpp 
scene.cpp 
shader.cpp)

target_link_libraries(renderer 
${SDL2_LIBRARY} 
${SDL2main_LIBRARY} 
${tbb12_LIBRARY} 
${glm_LIBRARY} 
${assimp-vc143-mt_LIBRARY})

add_compile_definitions(MSAA4X)