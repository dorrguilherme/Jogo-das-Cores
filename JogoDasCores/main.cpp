/*
*
*					Programa desenvolvido por Guilherme Mathias Dörr
*					Universidade do Vale do Rio dos Sinos - UNISINOS
*					Curso de Ciência da Computação - Processamento Gráfico
*					M3 | Tarefa - Jogo das Cores
*
*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>
using namespace std;

int points = 0, tentativa = 0, pointsTentativa = 0;
const int quadradosX = 20, quadradosY = 20, porcentagem = 8;

GLuint shaderProgram;
GLuint VAO, VBO;

struct quadrado {
    float red;
    float green;
    float blue;
    bool visible;
};

quadrado quadrados[quadradosX][quadradosY];

void init(void) {
    // geração de valores aleatórios para atribuição das cores
    cout << "Bem vindo ao Jogo das Cores" << endl;
    cout << "Voce tem 3 tentativas." << endl;

    random_device gerador;
    mt19937 gen(gerador());
    uniform_real_distribution<float> distr(0.0f, 1.0f);

    for (int i = 0; i < quadradosX; i++) {
        for (int j = 0; j < quadradosY; j++) {
            quadrados[i][j].red = distr(gen);
            quadrados[i][j].green = distr(gen);
            quadrados[i][j].blue = distr(gen);
            quadrados[i][j].visible = true;
        }
    }
}

void drawQuadrado(GLfloat x, GLfloat y, GLfloat size, const GLfloat* color) {
    GLfloat vertices[] = {
        x, y,
        x + size, y,
        x + size, y + size,
        x, y + size
    };

    glBindVertexArray(VAO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);

    glUseProgram(shaderProgram);
    glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, color);

    glDrawArrays(GL_POLYGON, 0, 4);

    glDisableVertexAttribArray(0);

    glBindVertexArray(0);
}

void analiseCores(int x, int y) {

    float width = 2.0f / quadradosX;
    float height = 2.0f / quadradosY;


    if (quadrados[x][y].visible) {
        float red = quadrados[x][y].red;
        float green = quadrados[x][y].green;
        float blue = quadrados[x][y].blue;
        double distancia = (porcentagem * 4.4167) / 100;
        //double distancia = (porcentagem * 100) / 441.67;
        //cout << "distancia: " << distancia << endl;
        int quantidadeVisiveis = 0;
        // Varredura da matriz para verificação de proximidade
        for (int i = 0; i < quadradosX; i++) {
            for (int j = 0; j < quadradosY; j++) {
                if (quadrados[i][j].visible) {
                    float quadradoRed = quadrados[i][j].red;
                    float quadradoGreen = quadrados[i][j].green;
                    float quadradoBlue = quadrados[i][j].blue;

                    // Calcula a diferença entre as cores
                    float diffRed = red - quadradoRed;
                    float diffGreen = green - quadradoGreen;
                    float diffBlue = blue - quadradoBlue;

                    // Calcula a distância euclidiana entre as cores
                    double distanciaCores = sqrt(diffRed * diffRed + diffGreen * diffGreen + diffBlue * diffBlue);
                    //cout << "distancia Cores: " << distanciaCores << endl;

                    // Verifica se a distância está dentro do limite
                    if (distanciaCores < distancia) {
                        quadrados[i][j].visible = false;
                        pointsTentativa++;
                    }
                }
            }
        }
        for (int i = 0; i < quadradosX; i++) {
            for (int j = 0; j < quadradosY; j++) {
                float x = 1.0f + i * width;
                float y = 1.0f - j * height;
                bool visible = quadrados[i][j].visible;
                GLfloat color[] = { quadrados[i][j].red, quadrados[i][j].green, quadrados[i][j].blue };
                drawQuadrado(x, y, width, color);
            }
        }
    }
    else {
        cout << "Errou :( " << endl;
    }
    glfwPostEmptyEvent();
}

void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && tentativa < 3) {
        //cout << "Botao pressionado" << endl;
        double xpos, ypos;

        glfwGetCursorPos(window, &xpos, &ypos);

        if (xpos >= 0 && xpos <= 800 && ypos >= 0 && ypos <= 600) {
            // Calcula a posição do quadrado com base nas coordenadas do clique
            int posicaoX = static_cast<int>(xpos / (800.0 / quadradosX));
            int posicaoY = static_cast<int>(ypos / (600.0 / quadradosY)) + 1;
            //cout << "posicao X: " << posicaoX << endl;
            //cout << "posicao Y: " << posicaoY << endl;

            // Realiza a análise das cores e atualiza o jogo
            analiseCores(posicaoX, posicaoY);

            tentativa++;
            if (pointsTentativa > 35) {
                cout << "Parabens! Pontuacao: " << pointsTentativa << endl;
            }
            else {
                cout << "Tente novamente, pontuacao: " << pointsTentativa << endl;
            }
            points += pointsTentativa;
            pointsTentativa = 0;
        }
    }
    else if (tentativa == 3) {
        if (points < 140) {
            cout << "Foi bom, mas voce consegue mais. Nas tres tentativas, voce totalizou " << points << " pontos!" << endl;
        } else {
            cout << "Parabens!! Nas tres tentativas, voce totalizou " << points << " pontos!" << endl;
        }
    }
}

void desenharQuadrados() {
    float width = 2.0f / quadradosX;
    float height = 2.0f / quadradosY;

    GLfloat backgroundColor[] = { 0.0f, 0.0f, 0.0f };

    for (int i = 0; i < quadradosX; i++) {
        for (int j = 0; j < quadradosY; j++) {
            float x = -1.0f + i * width;
            float y = 1.0f - j * height;

            if (quadrados[i][j].visible) {
                GLfloat color[] = { quadrados[i][j].red, quadrados[i][j].green, quadrados[i][j].blue };
                drawQuadrado(x, y, width, color);
            }
            else {
                drawQuadrado(x, y, width, backgroundColor);
            }
        }
    }
}

int main() {
    if (!glfwInit()) {
        cout << "Falha ao inicializar GLFW" << endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Jogo das Cores - Guilherme Mathias Dorr", nullptr, nullptr);
    if (!window) {
        cout << "Falha ao criar janela GLFW" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        cout << "Falha ao inicializar GLEW" << endl;
        glfwTerminate();
        return -1;
    }

    glfwSetMouseButtonCallback(window, glfwMouseButtonCallback);

    init();

    // Compilação do shader de vértice
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        void main() {
            gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
        }
    )";
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Compilação do shader de fragmento
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderSource = R"(
        #version 330 core
        uniform vec3 color;
        out vec4 FragColor;
        void main() {
            FragColor = vec4(color, 1.0);
        }
    )";
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Criação do programa de shader
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Criação do Vertex Array Object (VAO)
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Criação do buffer de vértices
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Configuração do VAO
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        desenharQuadrados();
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
