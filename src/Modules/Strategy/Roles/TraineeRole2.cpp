#include "TraineeRole2.h"
#include "Core/Utils/Math.h"
#include "Core/Utils/RelativeCoord.h"
#include "Core/Utils/RobotDefs.h"
#include "Core/Utils/CartesianCoord.h"

#define MAX_ANGLE 12
#define HEAD_PITCH 13

float headAngleX1 = 0, headAngleX2 = 0;
int cicles = 0;
bool flag = true, viuABola = false;
int tempoVendoBola = 0, tempoAndando = 0;


TraineeRole2::TraineeRole2(SpellBook *spellBook) : Role(spellBook)
{
    onStart = false;
}
TraineeRole2::~TraineeRole2()
{
}
void TraineeRole2::Tick(float ellapsedTime, const SensorValues &sensor)
{
    spellBook->strategy.MoveHead = false;
    spellBook->motion.HeadYaw = 0;
    spellBook->motion.HeadSpeedYaw = 0.09f;
    spellBook->motion.HeadSpeedPitch = 0.09f;

    if ((spellBook->strategy.GameState == STATE_READY || spellBook->strategy.GameState == STATE_PLAYING) && !onStart)
    {
        if (spellBook->perception.vision.localization.Enabled)
        {
            spellBook->strategy.WalkForward = true;
            spellBook->strategy.TargetX = -1.0f;
            spellBook->strategy.TargetY = 0.0f;
            spellBook->strategy.TargetTheta = 0;

            CartesianCoord current(spellBook->perception.vision.localization.X, spellBook->perception.vision.localization.Y);
            CartesianCoord desired(spellBook->strategy.TargetX, spellBook->strategy.TargetY);
            if (current.distance(desired) < 0.2f)
            {
                spellBook->strategy.WalkForward = false;
                onStart = true;
            }
        }
        else
        {
            onStart = true;
        }
    }
    if (spellBook->strategy.GameState == STATE_PLAYING && onStart)
    {
        cout << "na role trainee2, " << endl;

        if(spellBook->perception.vision.ball.BallDetected){
            tempoVendoBola++;
            if(tempoVendoBola >= 8) {
                viuABola = true;
            }
        } else {
            tempoVendoBola = 0;
        }

        if(!viuABola) {
            spellBook->motion.Vx = 0.0;
            spellBook->motion.HeadPitch = Deg2Rad(HEAD_PITCH);
            
            if(cicles <= 200) {
                //Procurar a Bola no eixo X
                cicles++;
                if(headAngleX1 <= MAX_ANGLE && flag) {
                    spellBook->motion.HeadYaw = Deg2Rad(headAngleX1);
                    headAngleX1 += 1;
                    cout << headAngleX1 << "\n";
                    if(headAngleX1 >= MAX_ANGLE) {
                        flag = false;
                    }
                }

                if(headAngleX2 >= -MAX_ANGLE && !flag) {
                    spellBook->motion.HeadYaw = Deg2Rad(headAngleX2);
                    headAngleX2 -= 1;
                    cout << headAngleX1 << "\n";
                    if(headAngleX1 <= -MAX_ANGLE) {
                        flag = true;
                    }
                }
                
            } else if(cicles <= 400){
                cicles++;
                tempoAndando += 0.2;
                spellBook->motion.Vx = 0.15;
                if(tempoAndando >= 100) {
                    spellBook->motion.Vx = 0.0;
                    tempoAndando = 0;
                }   
            } else {
                //Gira o NAO em torno do próprio eixo para procurar atrás
                spellBook->motion.HeadYaw = Deg2Rad(0);
                spellBook->motion.Vth = 0.3;
                cicles = 0;
            }

        } else {
            // Anda em linha reta e reseta os ângulos da cabeça 
            spellBook->motion.Vx = 0.15;
            spellBook->motion.Vth = 0;
            spellBook->motion.HeadPitch = Deg2Rad(HEAD_PITCH);
            spellBook->motion.HeadYaw = Deg2Rad(0);
            cicles = 0;
        }

        

        /*
        Os slides do trainee falam que o robô vai ser testado com a bola em distância curta, média, longa e atrás dele, teóricamente esse código cobre tudo isso, vamo testando e arrumando até ir
        */
        
        // informacoes disponiveis:
            //spellBook->motion.Vth = Deg2Rad(0); // SETA A VELOCIDADE ANGULAR PARA 0 GRAUS
            //spellBook->motion.Vx = 0; // SETA A VELOCIDADE LINEAR PARA 0 m/s (NAO COLOQUE MAIS QUE 0.2m/s!!!)
            // spellBook->perception.vision.ball.BallDetected // SE ESTA VENDO A BOLA
            // spellBook->perception.vision.ball.BallDistance // DISTANCIA ATE A BOLA em metros
            // spellBook->perception.vision.ball.BallYaw > Deg2Rad(10.0f) // ANGULACAO DA BOLA EM X
            //spellBook->motion.HeadPitch = Deg2Rad(24.0f); // ANGULACAO DA CABECA DO ROBO, POSITIVO O ROBO OLHA PRA BAIXO, NEGATIVO PRA CIMA
            // spellBook->motion.HeadPitch = Deg2Rad(24.0f); // OLHA PRA BAIXO
            //Tamanho = 57.3cm
    }
}
