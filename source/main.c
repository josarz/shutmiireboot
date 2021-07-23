#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <wiiuse/wpad.h>

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

//---------------------------------------------------------------------------------
int main(int argc, char **argv) {
//---------------------------------------------------------------------------------

	// Initialise the video system
	VIDEO_Init();

	// This function initialises the attached controllers
	WPAD_Init();
	// Everything with PAD is for Gamecube Controller -> This might crash the application, therefor it is commented out
	//PAD_Init();

	// Obtain the preferred video mode from the system
	// This will correspond to the settings in the Wii menu
	rmode = VIDEO_GetPreferredMode(NULL);

	// Allocate memory for the display in the uncached region
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

	// Initialise the console, required for printf
	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);

	// Set up the video registers with the chosen mode
	VIDEO_Configure(rmode);

	// Tell the video hardware where our display memory is
	VIDEO_SetNextFramebuffer(xfb);

	// Make the display visible
	VIDEO_SetBlack(FALSE);

	// Flush the video register changes to the hardware
	VIDEO_Flush();

	// Wait for Video setup to complete
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();


	// The console understands VT terminal escape codes
	// This positions the cursor on row 2, column 0
	// we can use variables for this with format codes too
	// e.g. printf ("\x1b[%d;%dH", row, column );
	printf("\x1b[2;0H");
	printf("\n xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
	printf("\n             Hello World, you see this is my first Homebrew                     \n");
	printf("\n             If you use the Wiimote                                             \n");
	printf("\n             Press A to Shutdown your Wii without WC24 (red light)             ");
	printf("\n             Press B to Shutdown your Wii with Wc24 (orange light)             ");
	printf("\n             Press 1 to reboot your Wii System                                 ");
	printf("\n             Press 2 to return to your Wii Menu                                 ");
	printf("\n             Press - or Home or + to return to the HBC                         \n");
	// printf("\n             If you use the Gamecube Controller                                 \n");
	// printf("\n             Press A to Shutdown your Wii without WC24 (red light)             ");
	// printf("\n             Press B to Shutdown your Wii with Wc24 (orange light)             ");
	// printf("\n             Press Left to reboot your Wii System                                 ");
	// printf("\n             Press Right to return to your WIi Menu                             ");
	// printf("\n             Press Start to return to the HBC                                     \n");
	printf("\n                         Thanks for using !ShutMiiReboot!                      \n");
	printf("\n xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

	while(1) {

	        // Call WPAD_ScanPads each loop, this reads the latest controller states
	        WPAD_ScanPads();
	        //PAD_ScanPads();
	        // WPAD_ButtonsDown tells us which buttons were pressed in this loop
	        // this is a "one shot" state which will not fire again until the button has been released
	        u32 pressed = WPAD_ButtonsDown(0);
	        //int buttonsDown = PAD_ButtonsDown(0);


	        // We return to the launcher application via exit
	        if ( pressed & WPAD_BUTTON_A ) SYS_ResetSystem(SYS_POWEROFF_STANDBY, 0, 0);
	        if ( pressed & WPAD_BUTTON_B ) SYS_ResetSystem(SYS_POWEROFF, 0, 0);
	        if ( pressed & WPAD_BUTTON_HOME) exit(0);
	        if ( pressed & WPAD_BUTTON_1 ) SYS_ResetSystem(SYS_RESTART, 0, 0);
	        if ( pressed & WPAD_BUTTON_PLUS ) exit(0);
	        if ( pressed & WPAD_BUTTON_2) SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);
	        if ( pressed & WPAD_BUTTON_MINUS ) exit(0);
	        // if ( buttonsDown & PAD_BUTTON_A ) SYS_ResetSystem(SYS_POWEROFF_STANDBY, 0, 0);
	        // if ( buttonsDown & PAD_BUTTON_B ) SYS_ResetSystem(SYS_POWEROFF, 0, 0);
	        // if ( buttonsDown & PAD_TRIGGER_R ) SYS_ResetSystem(SYS_RESTART, 0, 0);
	        // if ( buttonsDown & PAD_TRIGGER_L ) SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);
	        // if ( buttonsDown & PAD_BUTTON_START )exit(0);


	        // Wait for the next frame
	        VIDEO_WaitVSync();
	}

	return 0;
}
