#include "keyboard.h"


cKeyboard::cKeyboard() {
    active = false;
    keyboard_fd = 0;
    keyboard_ev = new input_event();
    keyboard_st = new keyboard_state();
    keyboard_fd = open(KEYBOARD_DEV, O_RDONLY | O_NONBLOCK);
    if (keyboard_fd > 0) {
        ioctl(keyboard_fd, EVIOCGNAME(256), name);
        std::cout << "   Name: " << name << std::endl;
        active = true;
        pthread_create(&thread, 0, &cKeyboard::loop, this);
    }
}

void* cKeyboard::loop(void *obj) {
    while (reinterpret_cast<cKeyboard *>(obj)->active) reinterpret_cast<cKeyboard *>(obj)->readEv();
}
 
void cKeyboard::readEv() {
    int bytes = read(keyboard_fd, keyboard_ev, sizeof(*keyboard_ev));
    if (bytes > 0) {
        if (keyboard_ev->type & EV_KEY) {
            keyboard_st->keys[keyboard_ev->code] = keyboard_ev->value;
            
        }
    }
}


short cKeyboard::getKeyState(short key) {
#if 0	
    return keyboard_st->keys[key];
#else
    short tmp = keyboard_st->keys[key];
    keyboard_st->keys[key] = KEY_RESERVED;
    return tmp;
#endif    
}

cKeyboard::~cKeyboard() {
    if (keyboard_fd > 0) {
        active = false;
        pthread_join(thread, 0);
        close(keyboard_fd);
    }
    delete keyboard_st;
    delete keyboard_ev;
    keyboard_fd = 0;
}

unsigned char clear_line[]={"\r                                                                \r"};
int main()
{
	cKeyboard kaidenkey;

	while(1){
		if (kaidenkey.getKeyState(KEY_UP))    printf("%sup !!!\n",clear_line);
		if (kaidenkey.getKeyState(KEY_DOWN))  printf("%sdown !!!\n",clear_line);
		if (kaidenkey.getKeyState(KEY_LEFT))  printf("%sleft !!!\n",clear_line);
		if (kaidenkey.getKeyState(KEY_RIGHT)) printf("%sright !!!\n",clear_line);
		if (kaidenkey.getKeyState(KEY_X)) printf("%sx !!!\n",clear_line);
		if (kaidenkey.getKeyState(KEY_Y)) printf("%sy !!!\n",clear_line);
		if (kaidenkey.getKeyState(KEY_Z)) printf("%sz !!!\n",clear_line);
		if (kaidenkey.getKeyState(KEY_1)) printf("%s1 !!!\n",clear_line);
		if (kaidenkey.getKeyState(KEY_2)) printf("%s2 !!!\n",clear_line);
		if (kaidenkey.getKeyState(KEY_3)) printf("%s3 !!!\n",clear_line);
		usleep(1); //make sure this while loop don't eat up all the CPU resource

	}
	
	return 0;
}