#include "TraineeRole2.h"
#include "Core/Utils/Math.h"
#include "Core/Utils/RelativeCoord.h"
#include "Core/Utils/RobotDefs.h"
#include "Core/Utils/CartesianCoord.h"

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
    spellBook->motion.HeadSpeedYaw = 0.2f;
    spellBook->motion.HeadSpeedPitch = 0.2f;

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

        /*
        Os slides do trainee falam que o robô vai ser testado com a bola em distância curta, média, longa e atrás dele, teóricamente esse código cobre tudo isso, vamo testando e arrumando até ir
        */

        //Se enxergar a bola
        if(spellBook->perception.vision.ball.BallDetected){
            //Enquanto a distancia do robô até a bola for maior do 0 (provavelmente vamos ter que mudar de 0 para compensar o fato de a medição ser feita a partir da câmera na cabeça dele)
            while(spellBook->perception.vision.ball.BallDistance > 0) {
                //O Samuel tinha citado algo sobre desacelerar o NAO, provavelmente teremos que diminuir a velocidade baseado na distancia da bola
                // spellBook->motion.Vx = (spellBook->perception.vision.ball.BallDistance * algum_fator)

                spellBook->motion.Vx = 0.15;
            }
            spellBook->motion.Vx = 0;
        } else {
            // Se não enxegar a bola vai procurar mais perto, ou mais longe 

            //Aumenta o ângulo da cabeça pra baixo, se achar a bola para
            for(float i = 0; i <= 24.0; i += 0.5) {
                spellBook->motion.HeadPitch = Deg2Rad(i);
                if(spellBook->perception.vision.ball.BallDetected)
                    break;
            }
            //Aumenta o ângulo da cabeça pra cima, se achar a bola para
            for(float i = 0; i >= -24.0; i -= 0.5) {
                spellBook->motion.HeadPitch = Deg2Rad(i);
                if(spellBook->perception.vision.ball.BallDetected)
                    break;
            }

            //Se não encontrar vai procurar em volta

            spellBook->motion.HeadPitch = Deg2Rad(0); // Zerando o angulo da cabeça, acho que vamos ter que colocar um ãngulo melhor depois
            while(!spellBook->perception.vision.ball.BallDetected){
                spellBook->motion.Vth = Deg2Rad(25); //Velocidade arbitrária, vamos testar uma mais otimizada.
            }
        }
        

        // informacoes disponiveis:
            //spellBook->motion.Vth = Deg2Rad(0); // SETA A VELOCIDADE ANGULAR PARA 0 GRAUS
            //spellBook->motion.Vx = 0; // SETA A VELOCIDADE LINEAR PARA 0 m/s (NAO COLOQUE MAIS QUE 0.2m/s!!!)
            // spellBook->perception.vision.ball.BallDetected // SE ESTA VENDO A BOLA
            // spellBook->perception.vision.ball.BallDistance // DISTANCIA ATE A BOLA em metros
            // spellBook->perception.vision.ball.BallYaw > Deg2Rad(10.0f) // ANGULACAO DA BOLA EM X
            //spellBook->motion.HeadPitch = Deg2Rad(24.0f); // ANGULACAO DA CABECA DO ROBO, POSITIVO O ROBO OLHA PRA BAIXO, NEGATIVO PRA CIMA
            // spellBook->motion.HeadPitch = Deg2Rad(24.0f); // OLHA PRA BAIXO
    }
}
