#include "types.h"

// =========================================================
// ADMIN MODE FUNCTIONS
// Handles secure entry into admin mode via a hardware switch,
// password verification via the keypad, interactive RTC time
// editing, and enabling/disabling scheduled messages.
// =========================================================

// Configures the admin switch pin as input and the buzzer pin
// as output. Call once during system startup (before the main loop).
void InitAdminMode(void);

// Reads the current state of the admin switch pin.
// Returns 1 if the switch is pressed (admin mode requested), 0 otherwise.
u8 AdminKeyPressed(void);

// Top-level admin mode handler. Called when the switch is pressed.
// Prompts for the password, and if correct, shows the admin menu
// (edit time / edit messages) until the user exits with 'C'.
void EnterAdminMode(void);

// Prompts the user to enter a numeric password on the keypad
// (digits followed by 'C' to confirm, using the existing ReadNum()
// routine). Returns 1 if it matches ADMIN_PASSWORD, 0 otherwise.
u8 CheckPassword(void);

// Interactively prompts for a new hour (0-23) and minute (0-59)
// via the keypad, then writes the new time to the RTC.
// Seconds are reset to 0 whenever the time is changed.
void EditRTCTimeInteractive(void);

// Lets the user page through all TOTAL_MESSAGES entries in
// messageList[], showing each one's ON/OFF status and a preview
// of its text, and toggle whether it is active for the day.
void EditMessagesMenu(void);

// Turns the buzzer on for the given duration (in milliseconds),
// then turns it off. Used for simple audible feedback
// (key press, wrong password, toggle confirmation, etc).
void Buzz(u32 ms);
