#define GL_SILENCE_DEPRECATION
#define TEXTURES_SIZE 22

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <cmath>

/**
* Author: [Pablo O'Hop]
* Assignment: [The Great Escape]
* Date due: 2024-05-03, 3:00pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

SDL_Window* displayWindow;
bool gameIsRunning = true;
bool showMenu = true, showWin = false, showLose = false;
bool soundPlayed = false;

GLuint textures[22];
GLuint levelBackgrounds[5];
int currentLevel = 0;
std::vector<std::vector<SDL_Rect>> imagePositions;
std::vector<std::vector<int>> textureIndices;
std::vector<int> correctImages;
std::vector<std::vector<int>> directions;

GLuint questionTextures[5];
bool showQuestion = true;

Mix_Music* backgroundMusic;
Mix_Chunk* clickSound;

void loadTexture(GLuint& texture, const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    assert(surface != nullptr);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_FreeSurface(surface);
}

void setup() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("CS3113 Escape", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

    IMG_Init(IMG_INIT_PNG);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    backgroundMusic = Mix_LoadMUS("/Users/pabloohop/Desktop/SDLSimple/assets/background_music.mp3");
    assert(backgroundMusic != nullptr);

    clickSound = Mix_LoadWAV("/Users/pabloohop/Desktop/SDLSimple/assets/click_sound.wav");
    assert(clickSound != nullptr);
    
    loadTexture(levelBackgrounds[0], "/Users/pabloohop/Desktop/SDLSimple/assets/room_level1.jpeg");
    loadTexture(levelBackgrounds[1], "/Users/pabloohop/Desktop/SDLSimple/assets/room_level2.jpeg");
    loadTexture(levelBackgrounds[2], "/Users/pabloohop/Desktop/SDLSimple/assets/room_level3.jpeg");
    loadTexture(levelBackgrounds[3], "/Users/pabloohop/Desktop/SDLSimple/assets/room_level4.jpeg");
    loadTexture(levelBackgrounds[4], "/Users/pabloohop/Desktop/SDLSimple/assets/room_level10.jpeg");
    
    loadTexture(textures[20], "/Users/pabloohop/Desktop/SDLSimple/assets/Background_2.png");
    loadTexture(textures[19], "/Users/pabloohop/Desktop/SDLSimple/assets/WinScreen.jpg");
    loadTexture(textures[18], "/Users/pabloohop/Desktop/SDLSimple/assets/LoseScreen.jpg");
    
    loadTexture(textures[0], "/Users/pabloohop/Desktop/SDLSimple/assets/Opening/OpeningResponse.jpg");
    loadTexture(textures[1], "/Users/pabloohop/Desktop/SDLSimple/assets/Question 1/EscapeAnswer1.1C.jpg");
    loadTexture(textures[2], "/Users/pabloohop/Desktop/SDLSimple/assets/Question 1/EscapeAnswer1.2.jpg");
    loadTexture(textures[3], "/Users/pabloohop/Desktop/SDLSimple/assets/Question 2/EscapeAnswer2.1.jpg");
    loadTexture(textures[4], "/Users/pabloohop/Desktop/SDLSimple/assets/Question 2/EscapeAnswer2.2.jpg");
    loadTexture(textures[5], "/Users/pabloohop/Desktop/SDLSimple/assets/Question 2/EscapeAnswer2.3C.jpg");
    loadTexture(textures[6], "/Users/pabloohop/Desktop/SDLSimple/assets/Question 3/EscapeAnswer3.1C.jpg");
    loadTexture(textures[7], "/Users/pabloohop/Desktop/SDLSimple/assets/Question 3/EscapeAnswer3.2.jpg");
    loadTexture(textures[8], "/Users/pabloohop/Desktop/SDLSimple/assets/Question 3/EscapeAnswer3.3.jpg");
    loadTexture(textures[9], "/Users/pabloohop/Desktop/SDLSimple/assets/Question 3/EscapeAnswer3.4.jpg");
    loadTexture(textures[10], "/Users/pabloohop/Desktop/SDLSimple/assets/Question 4/EscapeAnswer4.1.jpg");
    loadTexture(textures[11], "/Users/pabloohop/Desktop/SDLSimple/assets/Question 4/EscapeAnswer4.2C.jpg");
    loadTexture(textures[12], "/Users/pabloohop/Desktop/SDLSimple/assets/Question 4/EscapeAnswer4.3.jpg");
    loadTexture(textures[13], "/Users/pabloohop/Desktop/SDLSimple/assets/Question 4/EscapeAnswer4.4.jpg");
    loadTexture(textures[14], "/Users/pabloohop/Desktop/SDLSimple/assets/Question 4/EscapeAnswer4.5.jpg");
    
    
    
    loadTexture(questionTextures[0], "/Users/pabloohop/Desktop/SDLSimple/assets/Opening/OpeningMessage.jpg");
    loadTexture(questionTextures[1], "/Users/pabloohop/Desktop/SDLSimple/assets/Question 1/EscapeQuestion1.jpg");
    loadTexture(questionTextures[2], "/Users/pabloohop/Desktop/SDLSimple/assets/Question 2/EscapeQuestion2.jpg");
    loadTexture(questionTextures[3], "/Users/pabloohop/Desktop/SDLSimple/assets/Question 3/EscapeQuestion3.jpg");
    loadTexture(questionTextures[4], "/Users/pabloohop/Desktop/SDLSimple/assets/Question 4/EscapeQuestion4.jpg");
    
    imagePositions.resize(5);
    textureIndices.resize(5);
    directions.resize(5);
    correctImages = {0, 0, 2, 0, 1, 3};

    imagePositions[0].push_back({320 - 50, 240 - 50, 100, 100});
    textureIndices[0].push_back(0);

    imagePositions[1].push_back({100, 240 - 50, 100, 100});
    textureIndices[1].push_back(1);
    directions[1].push_back(1);
    imagePositions[1].push_back({540 - 100, 240 - 50, 100, 100});
    textureIndices[1].push_back(2);
    directions[1].push_back(-1);

    imagePositions[2].push_back({100, 240 - 50, 100, 100});
    textureIndices[2].push_back(3);
    directions[2].push_back(1);
    imagePositions[2].push_back({540 - 100, 240 - 50, 100, 100});
    textureIndices[2].push_back(4);
    directions[2].push_back(-1);
    imagePositions[2].push_back({320 - 50, 100, 100, 100});
    textureIndices[2].push_back(5);
    directions[2].push_back(1);

    imagePositions[3].push_back({100, 100, 100, 100});
    textureIndices[3].push_back(6);
    directions[3].push_back(1);
    imagePositions[3].push_back({100, 380 - 100, 100, 100});
    textureIndices[3].push_back(7);
    directions[3].push_back(1);
    imagePositions[3].push_back({540 - 100, 100, 100, 100});
    textureIndices[3].push_back(8);
    directions[3].push_back(-1);
    imagePositions[3].push_back({540 - 100, 380 - 100, 100, 100});
    textureIndices[3].push_back(9);
    directions[3].push_back(-1);

    imagePositions[4].push_back({50, 100, 100, 100});
    textureIndices[4].push_back(10);
    directions[4].push_back(1);

    imagePositions[4].push_back({590 - 100, 100, 100, 100});
    textureIndices[4].push_back(11);
    directions[4].push_back(-1);
    imagePositions[4].push_back({50, 380, 100, 100});
    textureIndices[4].push_back(12);
    directions[4].push_back(1);
    imagePositions[4].push_back({320 - 50, 20, 100, 100});
    textureIndices[4].push_back(13);
    directions[4].push_back(1);
    imagePositions[4].push_back({320 - 50, 460 - 100, 100, 100});
    textureIndices[4].push_back(14);
    directions[4].push_back(-1);
    
    
}

void updateImagePositions() {
    float deltaTime = 0.016f;
    const float speed = 100.0f * deltaTime;
    int boundaryOffset = 10;

    for (size_t level = 0; level < imagePositions.size(); ++level) {
        for (size_t i = 0; i < imagePositions[level].size(); ++i) {
            SDL_Rect& rect = imagePositions[level][i];
            int& dir = directions[level][i];

            if (level == 1 || level == 2) {
                rect.x += dir * speed;
                if (rect.x < boundaryOffset || rect.x + rect.w > 640 - boundaryOffset) {
                    dir = -dir;
                    rect.x += dir * speed;
                }
            } else if (level == 3) {
                if (i < 2) {
                    rect.x += dir * speed;
                    if (rect.x < boundaryOffset || rect.x + rect.w > 640 - boundaryOffset) {
                        dir = -dir;
                        rect.x += dir * speed;
                    }
                } else {
                    rect.y += dir * speed;
                    if (rect.y < boundaryOffset || rect.y + rect.h > 480 - boundaryOffset) {
                        dir = -dir;
                        rect.y += dir * speed;
                    }
                }
            } else if (level == 4) {
                if (i < 3) {
                    rect.x += dir * speed;
                    if (rect.x < boundaryOffset || rect.x + rect.w > 640 - boundaryOffset) {
                        dir = -dir;
                        rect.x += dir * speed;
                    }
                } else {
                    rect.y += dir * speed;
                    if (rect.y < boundaryOffset || rect.y + rect.h > 480 - boundaryOffset) {
                        dir = -dir;
                        rect.y += dir * speed;
                    }
                }
            }
        }
    }
}

void renderMenu() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, textures[20]);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex2f(-1.0, 1.0);
        glTexCoord2f(1.0, 0.0); glVertex2f(1.0, 1.0);
        glTexCoord2f(1.0, 1.0); glVertex2f(1.0, -1.0);
        glTexCoord2f(0.0, 1.0); glVertex2f(-1.0, -1.0);
    glEnd();
    SDL_GL_SwapWindow(displayWindow);
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (!showMenu && !showWin && !showLose && !showQuestion) {
        if (currentLevel >= 0 && currentLevel < 5) {
            glBindTexture(GL_TEXTURE_2D, levelBackgrounds[currentLevel]);
            glBegin(GL_QUADS);
                glTexCoord2f(0.0, 0.0); glVertex2f(-1.0f, 1.0f);
                glTexCoord2f(1.0, 0.0); glVertex2f(1.0f, 1.0f);
                glTexCoord2f(1.0, 1.0); glVertex2f(1.0f, -1.0f);
                glTexCoord2f(0.0, 1.0); glVertex2f(-1.0f, -1.0f);
            glEnd();
        }
    }

    if (showMenu) {
        glBindTexture(GL_TEXTURE_2D, textures[20]);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0); glVertex2f(-1.0, 1.0);
            glTexCoord2f(1.0, 0.0); glVertex2f(1.0, 1.0);
            glTexCoord2f(1.0, 1.0); glVertex2f(1.0, -1.0);
            glTexCoord2f(0.0, 1.0); glVertex2f(-1.0, -1.0);
        glEnd();
    } else if (showWin) {
        glBindTexture(GL_TEXTURE_2D, textures[19]);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0); glVertex2f(-1.0, 1.0);
            glTexCoord2f(1.0, 0.0); glVertex2f(1.0, 1.0);
            glTexCoord2f(1.0, 1.0); glVertex2f(1.0, -1.0);
            glTexCoord2f(0.0, 1.0); glVertex2f(-1.0, -1.0);
        glEnd();
    } else if (showLose) {
        glBindTexture(GL_TEXTURE_2D, textures[18]);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0); glVertex2f(-1.0, 1.0);
            glTexCoord2f(1.0, 0.0); glVertex2f(1.0, 1.0);
            glTexCoord2f(1.0, 1.0); glVertex2f(1.0, -1.0);
            glTexCoord2f(0.0, 1.0); glVertex2f(-1.0, -1.0);
        glEnd();
    } else if (showQuestion) {
        glBindTexture(GL_TEXTURE_2D, questionTextures[currentLevel]);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0); glVertex2f(-1.0f, 1.0f);
            glTexCoord2f(1.0, 0.0); glVertex2f(1.0f, 1.0f);
            glTexCoord2f(1.0, 1.0); glVertex2f(1.0f, -1.0f);
            glTexCoord2f(0.0, 1.0); glVertex2f(-1.0f, -1.0f);
        glEnd();
    }

    if (!showMenu && !showQuestion && !showLose) {
        for (size_t i = 0; i < imagePositions[currentLevel].size(); ++i) {
            if (i >= textureIndices[currentLevel].size()) {
                break;
            }
            const SDL_Rect& rect = imagePositions[currentLevel][i];
            if (rect.w > 0 && rect.h > 0) {
                glBindTexture(GL_TEXTURE_2D, textures[textureIndices[currentLevel][i]]);
                glBegin(GL_QUADS);
                glTexCoord2f(0.0, 0.0); glVertex2f(rect.x / 320.0f - 1.0f, 1.0f - rect.y / 240.0f);
                glTexCoord2f(1.0, 0.0); glVertex2f((rect.x + rect.w) / 320.0f - 1.0f, 1.0f - rect.y / 240.0f);
                glTexCoord2f(1.0, 1.0); glVertex2f((rect.x + rect.w) / 320.0f - 1.0f, 1.0f - (rect.y + rect.h) / 240.0f);
                glTexCoord2f(0.0, 1.0); glVertex2f(rect.x / 320.0f - 1.0f, 1.0f - (rect.y + rect.h) / 240.0f);
                glEnd();
            }
        }
    }
    SDL_GL_SwapWindow(displayWindow);
}



void scrollScreen(int direction) {
    const float speed = 100.0f * 0.016f;
    for (size_t level = 0; level < imagePositions.size(); ++level) {
        for (size_t i = 0; i < imagePositions[level].size(); ++i) {
            SDL_Rect& rect = imagePositions[level][i];
            rect.x -= direction * speed;
        }
    }
}

void processInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            gameIsRunning = false;
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_RETURN) {
                if (showMenu) {
                    showMenu = false;
                    showQuestion = true;
                    continue;
                }
                if (showQuestion) {
                    showQuestion = false;
                    continue;
                }
            }
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                gameIsRunning = false;
            }
            if (event.key.keysym.sym == SDLK_LEFT) {
                scrollScreen(-1);
            }
            if (event.key.keysym.sym == SDLK_RIGHT) {
                scrollScreen(1);
            }
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (!showMenu && !showQuestion) {
                int x = event.button.x;
                int y = event.button.y;
                if (x >= 0 && x < 640 && y >= 0 && y < 480) {
                    Mix_PlayChannel(-1, clickSound, 0);
                    soundPlayed = true;
                }

                if (currentLevel < imagePositions.size() && !imagePositions[currentLevel].empty()) {
                    for (int i = 0; i < imagePositions[currentLevel].size(); i++) {
                        const SDL_Rect& rect = imagePositions[currentLevel][i];
                        if (x > rect.x && x < rect.x + rect.w && y > rect.y && y < rect.y + rect.h) {
                            if (i == correctImages[currentLevel]) {
                                currentLevel++;
                                if (currentLevel >= imagePositions.size()) {
                                    showWin = true;
                                    showMenu = showQuestion = false;
                                    break;
                                } else {
                                    showQuestion = true;
                                }
                            } else {
                                showLose = true;
                                showMenu = showQuestion = false;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

int main(int argc, char* argv[]) {
    setup();
    
    Mix_PlayMusic(backgroundMusic, -1);

    Mix_VolumeMusic(30);
    Mix_VolumeChunk(clickSound, 128);
    
    while (gameIsRunning) {
        processInput();
        if (showMenu) {
            renderMenu();
        } else {
            updateImagePositions();
            render();
        }
    }

    Mix_FreeMusic(backgroundMusic);
    Mix_FreeChunk(clickSound);

    SDL_Quit();
    return 0;
}
