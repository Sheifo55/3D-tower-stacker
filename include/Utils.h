#pragma once

#include <glad/glad.h>

float calculateDeltaTime();
void glCheckError(const char* file, int line);
#define GL_CHECK_ERROR() glCheckError(__FILE__, __LINE__)
