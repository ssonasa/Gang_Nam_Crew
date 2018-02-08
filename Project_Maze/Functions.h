#define SDATA       6    // Serial Input Data
#define STR_CLK     13   // Storage Register Clock(LATCH)
#define SHR_CLK     19   // Shift Register Clock
#define SHR_CLEAR   26   // Shift Register Clear

void allclear(void);
int init(void);
void release(void);
void set(int index);
void set8(uint8_t index);
void set16(uint16_t index);

