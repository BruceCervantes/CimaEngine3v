#include <iostream>
#include <Motor/Motor.hpp>
#include <Juego/Juego.hpp>
#include <Motor/Utils/Utils.hpp>
#include <SFML/Audio.hpp>

int main(int argc, char* argv[])
{
    CE::MotorConfig config;
    config.vW=1920;
    config.vH=1080;

    config.titulo="CimaEngine";
    std::cout<<"Iniciando "<<config.titulo<<"\n";
    CE::Motor motor{config,std::make_unique<IVJ::Juego>()};
    return motor.OnRun();
}