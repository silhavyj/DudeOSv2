#ifndef _KEYMAP_H_
#define _KEYMAP_H_

// key map for normal typing
static unsigned char key_map[] = {
    0,0x1B,'1','2','3','4','5','6','7','8','9','0','-','=','\b','\t',
    'q','w','e','r','t','y','u','i','o','p','[',']','\n',0x80,
    'a','s','d','f','g','h','j','k','l',';',047,0140,0x80,
    0134,'z','x','c','v','b','n','m',',','.','/',0x80,
    '*',0x80,' ',0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
    0x80,0x80,0x80,0x80,'7','8','9',0x80,'4','5','6',0x80,
    '1','2','3','0',0177
};

// shifted key map
static unsigned char key_map_shifted[] = {
    0,033,'!','@','#','$','%','^','&','*','(',')','_','+','\b','\t',
	'Q','W','E','R','T','Y','U','I','O','P','{','}','\n',0x80,
	'A','S','D','F','G','H','J','K','L',':',042,'~',0x80,
	'|','Z','X','C','V','B','N','M','<','>','?',0x80,
	'*',0x80,' ',0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x80,0x80,0x80,0x80,'7','8','9',0x80,'4','5','6',0x80,
	'1','2','3','0',177
};

// caps-lock key map
static unsigned char key_map_caps_normal[] = {
    0,0x1B,'1','2','3','4','5','6','7','8','9','0','-','=','\b','\t',
    'Q','W','E','R','T','Y','U','I','O','P','[',']','\n',0x80,
    'A','S','D','F','G','H','J','K','L',';',047,0140,0x80,
    '|','Z','X','C','V','B','N','M',',','.','/',0x80,
    '*',0x80,' ',0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
    0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
    0x80,0x80,0x80,'0',177
};

// caps-lock + shift key map
static unsigned char key_map_caps_shifted[] = {
	0,033,'!','@','#','$','%','^','&','*','(',')','_','+','\b','\t',
	'q','w','e','r','t','y','u','i','o','p','{','}','\n',0x80,
	'a','s','d','f','g','h','j','k','l',':',042,'~',0x80,
	0134,'z','x','c','v','b','n','m','<','>','?',0x80,
	'*',0x80,' ',0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x80,0x80,0x80,0x80,'7','8','9',0x80,'4','5','6',0x80,
	'1','2','3','0',177
};

#endif