#include <iostream>

#include "lvgl.h"
#include "TFT_eSPI.h"
#include "string.h"


char example_string[] = "BG:#000000;LM:240,CEN,#000001,#000000;SC:240,#000000";

void parse_component(std::string component);
void parse_background(std::string bg_string);
void parse_linemeter(std::string lm_string);
void parse_scale(std::string scale_string);

struct time_watch_face{
    char type;
    char features[];
};

void parse_watch_face(char* info_string){

    char* components = strtok(info_string,";");
    std::string temp_components;
    while(components != NULL){
        std::cout<<"Component: "<<components<<std::endl;
        parse_component(components);
        components = strtok(NULL,";");
    }
}

void parse_component(std::string component){
    std::string component_type = component.substr(0,2);
    if(component_type.compare("BG") == 0){
        parse_background(component);
    }else if(component_type.compare("SC") == 0){
        std::cout<<"Scale"<<std::endl;
    }else if(component_type.compare("LM") == 0){
        parse_linemeter(component);
    }else{
        std::cerr<<"Bad Component"<<std::endl;
    }
}

void parse_background(std::string bg_string){

}

void parse_linemeter(std::string lm_string){

}

void parse_scale(std::string scale_string){

}

int main(){
    parse_watch_face(example_string);
    return 0;
}