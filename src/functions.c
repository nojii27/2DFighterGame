#include "../include/include.h"
#include "../include/structure.h"

int init(SDL_Window **window, SDL_Renderer **renderer, int w, int h)
{
    int imgFlags = IMG_INIT_JPG|IMG_INIT_PNG|IMG_INIT_TIF;

    if(0 != SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        return -1;
    }
    if (IMG_Init(imgFlags) != imgFlags)
    {
        fprintf(stderr,"Erreur lors de l'initialisation de la SDL_image : '%s'\n",IMG_GetError());
        return -1;
    }
    if(0 != SDL_CreateWindowAndRenderer(w, h, SDL_WINDOW_FULLSCREEN, window, renderer))
    {
        fprintf(stderr, "Erreur SDL_CreateWindowAndRenderer : %s", SDL_GetError());
        return -1;
    }
    return 0;
}

SDL_Texture *loadImage(const char path[], SDL_Renderer *renderer)
{
    SDL_Surface *tmp = NULL;
    SDL_Texture *texture = NULL;
    tmp = IMG_Load(path);
    if(NULL == tmp)
    {
        fprintf(stderr, "Erreur SDL_LoadBMP : %s", SDL_GetError());
        return NULL;
    }
    texture = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);
    if(NULL == texture)
    {
        fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s", SDL_GetError());
        return NULL;
    }
    return texture;
}

int setWindowColor(SDL_Renderer *renderer, SDL_Color color)
{
    if(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) < 0)
        return -1;
    if(SDL_RenderClear(renderer) < 0)
        return -1;
    return 0;
}


void updateEvent(struct Input *input)
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
            input->quit = SDL_TRUE;
        else if(event.type == SDL_KEYDOWN)
            input->key[event.key.keysym.scancode] = SDL_TRUE;
        else if(event.type == SDL_KEYDOWN)
            input->key[event.key.keysym.sym] = SDL_TRUE;
        else if(event.type == SDL_KEYUP)
            input->key[event.key.keysym.scancode] = SDL_FALSE;
        else if(event.type == SDL_MOUSEMOTION)
        {
            input->x = event.motion.x;
            input->y = event.motion.y;
            input->xrel = event.motion.xrel;
            input->yrel = event.motion.yrel;
        }
        else if(event.type == SDL_MOUSEWHEEL)
        {
            input->xwheel = event.wheel.x;
            input->ywheel = event.wheel.y;
        }
        else if(event.type == SDL_MOUSEBUTTONDOWN)
              input->mouse[event.button.button] = SDL_TRUE;
        else if(event.type == SDL_MOUSEBUTTONUP)
            input->mouse[event.button.button] = SDL_FALSE;

    }
}

int init_personnage(SDL_Renderer *renderer, personnages *personnage, char path[], int x, int y, int w, int h, int vitesse, int dstX)
{
    personnage->sprite = loadImage(path, renderer);
    if (personnage->sprite == NULL) {
        fprintf(stderr, "Error loadImage : %s", SDL_GetError());
        return -1;
    }
    personnage->sprite_sheet = IMG_Load(path);

    if (personnage->sprite_sheet == NULL ){
        fprintf(stderr, "Error IMG_Load : %s", SDL_GetError());
        return -1;
    }

    personnage->rect.x = x;
    personnage->rect.y = y;
    personnage->rect.w = w;
    personnage->rect.h = h;
    personnage->vitesse = vitesse;
    personnage->dst.x = dstX;
    personnage->dst.y = 700;
    personnage->dst.w = 300;
    personnage->dst.h = 300;
    personnage->vie = 151;
    personnage->mana = 0;

    return 0;
}

int fillActionPoint(const char *path, actionPoint *actionPoint, SDL_Renderer *renderer, int perso)
{

    int carac = 0,XorY = 0, Passeur = 0;
    int ligne = 0;
    FILE *file = NULL;

    file = fopen(path, "r");

    if (file == NULL) {
        fprintf(stderr, "Error FOPEN ");
        fclose(file);
        return 1;
    }
    do {
        carac = fgetc(file);
        if (carac == '\n')
            ligne++;
    } while (carac != EOF);
    rewind(file);
    actionPoint->ligne = ligne;

    actionPoint->action = malloc(sizeof(int*)*ligne);
    actionPoint->point = malloc(sizeof(int*)*ligne);
    for(int i=0;i<ligne;i++){
        actionPoint->action[i]= malloc(sizeof(int)*2);
        actionPoint->point[i]= malloc(sizeof(int)*2);
        actionPoint->action[i][0] = 0;
        actionPoint->action[i][1] = 0;
    }
    actionPoint->action[ligne]=NULL;
    actionPoint->point[ligne]=NULL;

    do{
        carac = fgetc(file);

        if(carac > 47 && carac < 58)
            actionPoint->action[Passeur][XorY] = (carac - 48 + actionPoint->action[Passeur][XorY]) * 10;
        else if(carac == 44) {
            actionPoint->action[Passeur][XorY] =  actionPoint->action[Passeur][XorY] / 10;
            XorY = 1;
        }
        else if (carac == '\n') {
            actionPoint->action[Passeur][XorY] =  actionPoint->action[Passeur][XorY] / 10;
            XorY = 0;
            Passeur++;
        }

    }while (carac != EOF);
    fclose(file);

    return 0;
}

void upPoint(actionPoint *actionPoint, personnages *personnages, int perso) // changer le nom de la focntion
{
    for (int i = 0; i < actionPoint->ligne; i++) {
        actionPoint->point[i][0] = (actionPoint->action[i][0] - (actionPoint->action[i][0]/80) * 80);
        actionPoint->point[i][1] = (actionPoint->action[i][1] - (actionPoint->action[i][1]/80) * 80);
    }
}

SDL_Color GetPixelColor(const SDL_Surface* pSurface, const int X, const int Y)
{

	const Uint8 Bpp = pSurface->format->BytesPerPixel;


	Uint8* pPixel = (Uint8*)pSurface->pixels + Y * pSurface->pitch + X * Bpp;

	Uint32 PixelData = *(Uint32*)pPixel;

	SDL_Color Color = {0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE};


	SDL_GetRGBA(PixelData, pSurface->format, &Color.r, &Color.g, &Color.b, &Color.a);

	return Color;
}
void changeSprite (personnages *personnage, int x, int y, int narutoOrSasuke){
     personnage->rect.x = x;
     personnage->rect.y = y;

}

int my_collisions(personnages *personnage, actionPoint actionPoint, SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *texture, int defender, int striker, int atck)
{
    int status = 0;
    SDL_Color c;
    int x = 0;
    int y = 0;


    if (personnage[0].dst.x + 200 > personnage[1].dst.x && personnage[0].dst.x < personnage[1].dst.x + 300) {

    }
    else
        return 0;

    if (SDL_SetClipRect(personnage[defender].sprite_sheet, &personnage[defender].rect) == SDL_FALSE) {
        fprintf(stderr, "Error SDL_SetClipRect : %s", SDL_GetError());
        return 0;
    }
    x = ((personnage[defender].sprite_sheet->clip_rect.x + 80) - (((personnage[striker].dst.x -(personnage[striker].dst.x / 80) * 80) - actionPoint.point[atck][0]))) + (personnage[defender].dst.x -(personnage[defender].dst.x / 80) * 80);
    y = (personnage[defender].sprite_sheet->clip_rect.y + 80) - (0) - actionPoint.point[atck][1];
    for (int i = -5; i <= 5; i++) {
        for (int j = -5; j <= 5; j++) {
            c = GetPixelColor(personnage[defender].sprite_sheet, x + j, y + i);
            if (c.a == 255) {
                status = 1;
                personnage[defender].collisionA = 1;

                return status;
            }

        }
    }

    return status;
}

int animation(SDL_Renderer *renderer, personnages personnage)
{
    SDL_RenderCopy(renderer, personnage.sprite, &personnage.rect, &personnage.dst);
    return 0;
}
void SetKeyToFalse(Input* in)
{
    in->key[SDL_SCANCODE_ESCAPE] = SDL_FALSE;
    in->key[SDL_SCANCODE_D] = SDL_FALSE;
    in->key[SDL_SCANCODE_A] = SDL_FALSE;
    in->key[SDL_SCANCODE_S] = SDL_FALSE;
    in->key[SDL_SCANCODE_SPACE]= SDL_FALSE;
    in->key[SDL_SCANCODE_W]=SDL_FALSE;
    in->key[SDL_SCANCODE_F] = SDL_FALSE;
    in->key[SDL_SCANCODE_E] = SDL_FALSE;
    in->key[SDL_SCANCODE_RIGHT] = SDL_FALSE;
    in->key[SDL_SCANCODE_DOWN] = SDL_FALSE;
    in->key[SDL_SCANCODE_UP] = SDL_FALSE;
    in->key[SDL_SCANCODE_LEFT]=SDL_FALSE;
    in->key[SDL_SCANCODE_1]=SDL_FALSE;
    in->key[SDL_SCANCODE_2]=SDL_FALSE;
    in->key[SDL_SCANCODE_3]=SDL_FALSE;
    in->key[SDLK_KP_1] = SDL_FALSE;
    in->key[SDLK_KP_0] = SDL_FALSE;
    in->key[SDLK_KP_2] = SDL_FALSE;
    in->key[SDL_SCANCODE_R] = SDL_FALSE;
    in->key[SDL_SCANCODE_W] = SDL_FALSE;
}

void animationP(personnages* personnage,int x , int y, int nombreAnimation, int reset_x, Input in)
{
    personnage->rect.y = y;
    personnage->rect.x += personnage->rect.w;
    if (personnage->rect.x >= (personnage->rect.w * nombreAnimation))
            personnage->rect.x = reset_x;


    SDL_Delay(37);
}
void BougerPerso(personnages *personnage, Direction *dir, Input *in)
{
    if (dir->right && personnage[0].dst.x < 1640)
        personnage[0].dst.x = personnage[0].dst.x + personnage[0].vitesse;

    if (dir->left && personnage[0].dst.x > 0)
        personnage[0].dst.x = personnage[0].dst.x - personnage[0].vitesse;


    if (dir->down && personnage[0].dst.y < 700) {
            while (personnage[0].dst.y < 700)
                personnage[0].dst.y = personnage[0].dst.y + personnage[0].vitesse / 2;
    }
    dir->right = SDL_FALSE;
    dir->left = SDL_FALSE;
    dir->down = SDL_FALSE;
}
void changeLife (personnages *personnage,int dmg, Input in){
    if(in.key[SDL_SCANCODE_W]|| in.key[SDL_SCANCODE_UP] ){}
    else
        personnage->vie -= dmg;

}


void changeMana (personnages *personnage,int quantity,int reduceOrAdd){
    if(reduceOrAdd == REDUCE)
    personnage->mana -= quantity;
    else if (reduceOrAdd == ADD && personnage->mana <= 130)
        personnage->mana += quantity;
}

int murInvisible(personnages *personnage, int narutoOrSasuke){

if (narutoOrSasuke == 0 && personnage[0].dst.x + (personnage[0].dst.w / 2) < personnage[1].dst.x + ( personnage[1].dst.w / 4))
    return 0;

if (narutoOrSasuke == 1 && personnage[1].dst.x + (personnage[1].dst.w / 4) > personnage[0].dst.x + ( personnage[0].dst.w / 2))
    return 0 ;

    return 1;
}



Direction narutoMove(Input in, personnages *personnage, Direction dir, SDL_Window *window, actionPoint actionPoint, SDL_Renderer *renderer, SDL_Texture *texture)
{

        if( in.key[SDL_SCANCODE_SPACE]) {
                animationP(&personnage[0], 0, 240, 3, 0,in);
                if(my_collisions(personnage, actionPoint, window, renderer, texture, 1, 0, 0) == 1){

                    changeLife(&personnage[1],6,in);
            }

        }
        else if (in.key[SDL_SCANCODE_W]){
                animationP(&personnage[0],0,720,1,0,in);


        }

        else if  (in.key[SDL_SCANCODE_F] && personnage[0].mana > 30 && personnage[1].rect.y != 640){
                animationP(&personnage[0],0,400,21,0,in);
                if(personnage[1].dst.x - personnage[0].dst.x < 200)
                    changeLife(&personnage[1],10,in);
                changeMana(&personnage[0], 6,REDUCE);

        }
        else if (in.key[SDL_SCANCODE_E]) {
                animationP(&personnage[0],0,560,4,0,in);
                if(my_collisions(personnage, actionPoint, window, renderer, texture, 1, 0, 1) == 1){

                    changeLife(&personnage[1],6,in);
            }
        }
        else if (in.key[SDL_SCANCODE_D] && murInvisible(personnage,0) == 0 ) {
                animationP(&personnage[0], 0, 80, 6, 0,in);
                dir.right = SDL_TRUE;
                BougerPerso(&personnage[0], &dir, &in);
        }
        else if (in.key[SDL_SCANCODE_A] ) {
                animationP(&personnage[0], 0, 160, 6, 0,in);
                dir.left = SDL_TRUE;
                BougerPerso(&personnage[0], &dir, &in);
        }
        else if (in.key[SDL_SCANCODE_S]){
                animationP(&personnage[0], 480, 0, 1, 480,in);
                changeMana(&personnage[0],2,ADD);

        }

        else if (in.key[SDL_SCANCODE_R]){
                animationP(&personnage[0],0,800,3,0,in);

        }
        else if(personnage[0].collisionA == 1){
                 animationP(&personnage[0],0,800,4,0,in);
                 personnage[0].collisionA = 0;
        }
        else {
                animationP(&personnage[0], 0, 0, 6, 0,in);

        }
        return dir;
}

Direction sasukeMove(Input in, personnages *personnage, Direction dir, SDL_Window *window, actionPoint actionPoint, SDL_Renderer *renderer, SDL_Texture *texture)
{

        if (in.key[SDL_SCANCODE_KP_0]) {
            animationP(&personnage[1],0,320,4,0,in);
            if(my_collisions(personnage, actionPoint, window, renderer, texture, 0, 1, 0) == 1){
                changeLife(&personnage[0],3,in);

        }
    }

        else if (in.key[SDL_SCANCODE_UP]){
            animationP(&personnage[1],0,640,1,0,in);
        }

        else if (in.key [SDL_SCANCODE_KP_2]&& personnage[1].mana > 30) {
            animationP(&personnage[1],0,400,15,0,in);
            if( my_collisions(personnage, actionPoint, window, renderer, texture, 0, 1, 1) == 1){
                changeLife(&personnage[0],10,in);

        }
        changeMana(&personnage[1],6,REDUCE);
    }
        else if(in.key[SDL_SCANCODE_KP_1]) {
            animationP(&personnage[1],0,560,3,0,in);
            if(my_collisions(personnage, actionPoint, window, renderer, texture, 0, 1, 2) == 1){

                changeLife(&personnage[0],3,in);
        }
    }
        else if (in.key[SDL_SCANCODE_LEFT] && murInvisible(personnage,1) == 0) {
            animationP(&personnage[1], 0, 80, 4, 0,in);
            BougerPerso(&personnage[1], &dir, &in);
            dir.left = SDL_TRUE;
        }
        else if (in.key[SDL_SCANCODE_DOWN]){
            animationP(&personnage[1], 320, 0, 1, 320,in);
            changeMana(&personnage[1],2,ADD);
        }
        else if (in.key[SDL_SCANCODE_RIGHT]){
            animationP(&personnage[1],0,240,4,0,in);
            BougerPerso(&personnage[1],&dir,&in);
            dir.right=SDL_TRUE;
        }
        else if(personnage[1].collisionA == 1){
                animationP(&personnage[1],0,720,4,0,in);
                personnage[1].collisionA = 0;

        }
        else
            animationP(&personnage[1], 0, 0, 4, 0, in);

        return dir;
}


void show_life(personnages *personnage, SDL_Renderer *renderer)
{
    SDL_Rect narutoLife;
    SDL_Rect sasukeLife;
    SDL_Rect dst;

    narutoLife.y = 80 * 12;
    narutoLife.x = 80 * 23;
    narutoLife.w = 80;
    narutoLife.h = 80;

    sasukeLife.y = 80 * 14;
    sasukeLife.x = 80 * 14;
    sasukeLife.w = 80;
    sasukeLife.h = 80;

    dst.x = 80;
    dst.y = 80;
    dst.w = 300;
    dst.h = 300;

    SDL_RenderCopy(renderer, personnage[0].sprite, &narutoLife, &dst);
    dst.x = 1920 - (dst.w + 80);
    SDL_RenderCopy(renderer, personnage[1].sprite, &sasukeLife, &dst);
}

void life(SDL_Renderer *renderer, personnages *personnage)
{
    SDL_Rect narutoHealthBar, narutoManaBar;
    SDL_Rect sasukeHealthBar, sasukeManaBar;
    int contrasteNaruto = 255 * ((personnage[0].vie ) / 151 ) ;
    int contrasteSasuke = 255 * ((personnage[1].vie ) / 151 ) ;
    int contrasteRougeNaruto = 255 - contrasteNaruto ;
    int contrasteRougeSasuke = 255 - contrasteSasuke ;

    narutoHealthBar.x = 180;
    narutoHealthBar.y = 90;
    narutoHealthBar.w = personnage[0].vie ;
    narutoHealthBar.h = 16;

    narutoManaBar.x = 180;
    narutoManaBar.y = 107;
    narutoManaBar.w = personnage[0].mana;
    narutoManaBar.h = 5;

    sasukeHealthBar.x = 1742;
    sasukeHealthBar.y = 90;
    sasukeHealthBar.w = 0 - personnage[1].vie ;
    sasukeHealthBar.h = 16;

    sasukeManaBar.x = 1743;
    sasukeManaBar.y = 106;
    sasukeManaBar.w = 0 - personnage[1].mana;
    sasukeManaBar.h = 5;

    SDL_SetRenderDrawColor(renderer, contrasteRougeNaruto, contrasteNaruto, 0, 0);
    if(personnage[0].vie >= 0)
        SDL_RenderFillRect(renderer, &narutoHealthBar);

    SDL_SetRenderDrawColor(renderer, contrasteRougeSasuke, contrasteSasuke, 0, 0);
    if (personnage[1].vie >= 0)
        SDL_RenderFillRect(renderer, &sasukeHealthBar);

    SDL_SetRenderDrawColor(renderer,0,128,255,255);
    if (personnage[0].mana >= 0)
        SDL_RenderFillRect(renderer,&narutoManaBar);

    SDL_SetRenderDrawColor(renderer,0,128,255,255);
    if (personnage[1].mana >= 0)
        SDL_RenderFillRect(renderer,&sasukeManaBar);


}

void gameOver(personnages *personnage, Input *in, SDL_Renderer *renderer){

    if (personnage[0].vie <= 0){
        //printf("Sasuke won ");
        SDL_Texture *texture = NULL;
        texture = loadImage("fonts/sasukeEnd.jpg", renderer);
    if (texture == NULL) {
        fprintf(stderr, "Error loadImage : %s", SDL_GetError());
        }
        in->quit = SDL_TRUE;
    }
    if (personnage[1].vie <= 0){
        //printf("Naruto won ");
        in->quit = SDL_TRUE;
    }
}

















