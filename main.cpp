#include <iostream>
#include <raylib.h>

using std::string;

Color Verde = {38, 185, 154, 255};
Color Amarelo = {223, 255, 117, 255};

int pontosJogador = 0;
int pontosCpu = 0;

// Classe da Bolinha

class Bola {
    public:
    float x,y;
    int velocidadeX, velocidadeY;
    int raio;

    void Mover () {
        DrawCircle(x, y, raio, Amarelo);
    }

    void AtualizarPos() {
        x += velocidadeX;
        y += velocidadeY;

        // Permite que a bolinha nao passe o limite da tela

        if (y + raio >= GetScreenHeight() || y - raio <= 0) {
            velocidadeY *= -1;
        }

        // Calcula os pontos do jogador / CPU

        if (x + raio >= GetScreenWidth()) {
            pontosCpu++;
            bolaResetPos();
        }

        if (x - raio <= 0) {
            pontosJogador++;
            bolaResetPos();
        }
    }

    void bolaResetPos() {
        x = GetScreenWidth()/2;
        y = GetScreenHeight()/2;

        int velAleatoria[2] = {-1,1};
        velocidadeX *= velAleatoria[GetRandomValue(0,1)];
        velocidadeY *= velAleatoria[GetRandomValue(0,1)];
    }
};

// Classe da Raquete do Jogador

class Raquetes {
protected:

    void LimitarMovimento() {

        if (y <= 0) {
            y = 0;
        }

        if (y + altura >= GetScreenHeight()) {
            y = GetScreenHeight() - altura;
        }
    }

public:
    float x,y;
    float altura,largura;
    int velocidade;

    void Mover() {
        DrawRectangle(x,y,largura,altura,WHITE);
    }

    void AtualizarPlayerPos() {
        // Cima / Baixo
        if (IsKeyDown(KEY_UP)) {
            y -= velocidade;
        }
        if (IsKeyDown(KEY_DOWN)) {
            y += velocidade;
        }
        LimitarMovimento();
    }
};

// Classe da CPU (Herda de Raquetes)

class RaqueteCPU : public Raquetes {
    public:

    void AtualizarCpuPos(float bolaY) {
        if (y + altura/2 > bolaY) {
            y -= velocidade;
        }
        if (y + altura/2 <= bolaY) {
            y += velocidade;
        }
        LimitarMovimento();
    }
};

Bola bola;
Raquetes player;
RaqueteCPU cpu;

int main() {
    // Definicao de tamanho da janela + FPS

    const int alturaTela = 800;
    const int larguraTela = 1200;

    InitWindow(larguraTela, alturaTela, "Ping! | Pong!");
    InitAudioDevice();
    Sound colBolinha = LoadSound("sfx/BarulhoBolinha.wav");
    SetTargetFPS(60);

    // Definicao de Valores

    bola.raio = 20;
    bola.x = larguraTela / 2;
    bola.y = alturaTela / 2;
    bola.velocidadeX = 7;
    bola.velocidadeY = 7;

    player.largura = 25;
    player.altura = 120;
    player.x = larguraTela - player.largura - 10;
    player.y = alturaTela / 2 - player.altura/2;
    player.velocidade = 6;

    cpu.largura = 25;
    cpu.altura = 120;
    cpu.x = 10;
    cpu.y = alturaTela/2 - cpu.altura/2;
    cpu.velocidade = 6;

    while (!WindowShouldClose()) { 

        // Updates

        bola.AtualizarPos();
        player.AtualizarPlayerPos();
        cpu.AtualizarCpuPos(bola.y);

        if (bola.velocidadeX > 0 && CheckCollisionCircleRec(Vector2{bola.x, bola.y}, bola.raio, Rectangle{player.x, player.y, player.largura, player.altura} )) {

            bola.velocidadeX *= -1;
            bola.x = player.x - bola.raio;
            PlaySound(colBolinha);
        }

        if (bola.velocidadeX < 0 && CheckCollisionCircleRec(Vector2{bola.x, bola.y}, bola.raio, Rectangle{cpu.x, cpu.y, cpu.largura, cpu.altura} )) {

            bola.velocidadeX *= -1;
            bola.x = cpu.x + cpu.largura + bola.raio;
            PlaySound(colBolinha);
        }

        // Main

        BeginDrawing();

        ClearBackground(Verde);

        DrawLine(larguraTela / 2, 0, larguraTela / 2, alturaTela, GRAY); // Linha do Meio

        bola.Mover();
        player.Mover();
        cpu.Mover();

        DrawText(TextFormat("%i", pontosCpu), larguraTela/4 -20, 20, 80, WHITE);
        DrawText(TextFormat("%i", pontosJogador), 3*larguraTela/4 -20, 20, 80, WHITE);

        EndDrawing();
    }

    UnloadSound(colBolinha);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
