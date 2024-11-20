
void mouse_handler() {
    // Step 1: Enable the auxiliary (mouse) device
    outb(0x64, 0xA8);

    // Step 2: Enable IRQ12 data reporting
    outb(0x64, 0xD4);      // Tell controller we're sending a mouse command
    outb(0x60, 0xF4);      // Send the enable data reporting command to the mouse

    // Step 3: Optional acknowledgment check
    // Wait and check if 0xFA is returned (success acknowledgment)
    // Use a read function here if you'd like to verify:
    unsigned char ack = inb(0x60);
    if (ack != 0xFA) {
        // Handle error (e.g., log or ignore)
    }

    // Mouse is now configured to send data on IRQ12.
}

void update_mouse_state(int x, int y, unsigned char buttons) {
    // Update your cursor position or handle button presses
    // For example:
    static int cursor_x = 0, cursor_y = 0;

    cursor_x += x;
    cursor_y -= y; // Typically, Y is inverted

    // Clamp values or apply bounds if needed
    if (cursor_x < 0) cursor_x = 0;
    if (cursor_y < 0) cursor_y = 0;

    // Print or debug the state
    // printf("Mouse X: %d, Y: %d, Buttons: %x\n", cursor_x, cursor_y, buttons);
}