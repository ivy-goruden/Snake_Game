#include "./ui_snake_desktop_render.h"
#include "snake_desktop_render.h"

bool s21::Snake_Desktop_Render::Waiting_Handler(int) { return false; };
bool s21::Snake_Desktop_Render::Move_Handler(int) { return false; };
bool s21::Snake_Desktop_Render::Pause_Handler(int) { return false; };
bool s21::Snake_Desktop_Render::Win_Handler(int) { return false; };
bool s21::Snake_Desktop_Render::Lose_Handler(int) { return false; };