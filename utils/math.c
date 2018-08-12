#include <stdlib.h>
float random_unit() {
    return 1/((float)(rand()%255));
}

float plus_unit(float raw, float step) {
    raw += step;
    while (1) {
        if (raw > 1) {
            raw =raw - 1;
        } else {
            return raw;
        }
    }
}