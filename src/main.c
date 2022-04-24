#include "../include/include.h"
#include "../include/structure.h"

int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;
    personnages personnage[2];
    actionPoint actionPoint[2];
    Direction dir[2];
    Input in;
    int statut = EXIT_FAILURE;
    in.quit = SDL_FALSE;
    SDL_Surface *icone = IMG_Load("fonts/946997_df8fa.png");


    if(0 != init(&window, &renderer, 1920, 1080))
        goto Quit;
    SDL_SetWindowIcon(window, icone);
    texture = loadImage("fonts/La_vallee_de_la_fin.png", renderer);
    if (texture == NULL) {
        fprintf(stderr, "Error loadImage : %s", SDL_GetError());
        goto Quit;
    }
    personnage[0].path = malloc(sizeof(char) * 29);
    personnage[0].path = "fonts/naruto-animation.png\0";
    if (init_personnage(renderer, &personnage[0], "fonts/naruto-animation.png", 0, 0, 80, 80, 70, 800) != 0) {
        fprintf(stderr, "Error init_personnage : %s", SDL_GetError());
        goto Quit;
    }
    personnage[1].path = malloc(sizeof(char) * 29);
    personnage[1].path = "fonts/sasuke-animation.png\0";
    if (init_personnage(renderer, &personnage[1], personnage[1].path, 0, 0, 80, 80, 70, 1000) != 0) {
        fprintf(stderr, "Error init_personnage : %s", SDL_GetError());
        goto Quit;
    }
    if (fillActionPoint("sasuke.txt", &actionPoint[0], renderer, 0) == 1)
        goto Quit;
    if (fillActionPoint("naruto.txt", &actionPoint[1], renderer, 1) == 1)
        goto Quit;
    upPoint(&actionPoint[0], personnage, 0);
    upPoint(&actionPoint[1], personnage, 1);
    SetKeyToFalse(&in);
    while(!in.quit)
    {

        SDL_RenderCopy(renderer, texture, NULL, NULL);
        updateEvent(&in);
        if(in.key[SDL_SCANCODE_ESCAPE])
                in.quit = SDL_TRUE;
        dir[0] = narutoMove(in, personnage, dir[0], window, actionPoint[1], renderer, texture);


        dir[1] = sasukeMove(in, personnage, dir[1], window, actionPoint[0], renderer, texture);


        animation(renderer, personnage[0]);
        animation(renderer, personnage[1]);


        show_life(personnage, renderer);
        life(renderer,personnage);
        SDL_RenderPresent(renderer);
        gameOver(personnage,&in,renderer);



    }
    statut = EXIT_SUCCESS;
    Quit:
    if (personnage[0].path != NULL)
        free(personnage[0].path);
    if (actionPoint[0].action != NULL)
        free(actionPoint[0].action);
    if (actionPoint[1].action != NULL)
        free(actionPoint[1].action);
    if (actionPoint[0].point != NULL)
        free(actionPoint[0].point);
    if (actionPoint[1].point != NULL)
        free(actionPoint[1].point);
    if (icone != NULL)
        SDL_FreeSurface(icone);
    if (personnage[0].sprite != NULL)
        SDL_DestroyTexture(personnage[0].sprite);
    if (personnage[1].sprite != NULL)
        SDL_DestroyTexture(personnage[1].sprite);
    if (texture != NULL)
        SDL_DestroyTexture(texture);
    if(NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if(NULL != window)
        SDL_DestroyWindow(window);
    if(personnage->sprite_sheet != NULL)
        SDL_FreeSurface(personnage->sprite_sheet);
    IMG_Quit();
    SDL_Quit();
    return statut;
}
