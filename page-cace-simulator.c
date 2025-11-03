#include <stdio.h>

#define MAX_PAGES 12
#define MAX_FRAMES 3

// Shared page reference string
int pages[MAX_PAGES] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};

// ======================== LRU PAGE REPLACEMENT ========================
static int findPage(int buffer[], int page, int n) {
    for (int i = 0; i < n; i++)
        if (buffer[i] == page)
            return i;
    return -1;
}

void simulateLRU() {
    int buffer[MAX_FRAMES];
    int time[MAX_FRAMES];
    int hits = 0, misses = 0, counter = 0;

    for (int i = 0; i < MAX_FRAMES; i++) {
        buffer[i] = -1;
        time[i] = 0;
    }

    printf("\n===== LRU Page Replacement =====\n\n");

    for (int i = 0; i < MAX_PAGES; i++) {
        int page = pages[i];
        int pos = findPage(buffer, page, MAX_FRAMES);

        if (pos != -1) {
            hits++;
            time[pos] = ++counter;
        } else {
            misses++;
            int empty = -1;
            for (int j = 0; j < MAX_FRAMES; j++)
                if (buffer[j] == -1)
                    empty = j;

            if (empty != -1) {
                buffer[empty] = page;
                time[empty] = ++counter;
            } else {
                int lru = 0;
                for (int j = 1; j < MAX_FRAMES; j++)
                    if (time[j] < time[lru])
                        lru = j;
                buffer[lru] = page;
                time[lru] = ++counter;
            }
        }

        printf("Step %2d (Page %d): ", i + 1, page);
        for (int j = 0; j < MAX_FRAMES; j++)
            buffer[j] == -1 ? printf("- ") : printf("%d ", buffer[j]);
        printf(pos != -1 ? "-> HIT\n" : "-> MISS\n");
    }

    printf("\nTotal Hits: %d\nTotal Misses: %d\nHit Ratio: %.2f\n",
           hits, misses, (float)hits / MAX_PAGES);
}
// ======================== CLOCK PAGE REPLACEMENT ========================
void simulateClock() {
    int buffer[MAX_FRAMES];
    int refBit[MAX_FRAMES];
    int pointer = 0, hits = 0, misses = 0;

    for (int i = 0; i < MAX_FRAMES; i++) {
        buffer[i] = -1;
        refBit[i] = 0;
    }

    printf("\n===== CLOCK Page Replacement =====\n\n");

    for (int i = 0; i < MAX_PAGES; i++) {
        int page = pages[i];
        int found = -1;

        for (int j = 0; j < MAX_FRAMES; j++) {
            if (buffer[j] == page) {
                found = j;
                break;
            }
        }

        if (found != -1) {
            hits++;
            refBit[found] = 1;
        } else {
            misses++;
            while (refBit[pointer] == 1) {
                refBit[pointer] = 0;
                pointer = (pointer + 1) % MAX_FRAMES;
            }
            buffer[pointer] = page;
            refBit[pointer] = 1;
            pointer = (pointer + 1) % MAX_FRAMES;
        }

        printf("Step %2d (Page %d): ", i + 1, page);
        for (int j = 0; j < MAX_FRAMES; j++)
            buffer[j] == -1 ? printf("- ") : printf("%d ", buffer[j]);
        printf(found != -1 ? "-> HIT\n" : "-> MISS\n");
    }

    printf("\nTotal Hits: %d\nTotal Misses: %d\nHit Ratio: %.2f\n",
           hits, misses, (float)hits / MAX_PAGES);
}

// ======================== ARC PAGE REPLACEMENT (Simplified) ========================
void simulateARC() {
    int T1[MAX_FRAMES] = {-1}, T2[MAX_FRAMES] = {-1};
    int hits = 0, misses = 0;

    printf("\n===== ARC (Simplified) Page Replacement =====\n\n");

    for (int i = 0; i < MAX_PAGES; i++) {
        int page = pages[i];
        int foundT1 = -1, foundT2 = -1;

        for (int j = 0; j < MAX_FRAMES; j++) {
            if (T1[j] == page) foundT1 = j;
            if (T2[j] == page) foundT2 = j;
        }

        if (foundT1 != -1 || foundT2 != -1) {
            hits++;
            if (foundT1 != -1) {
                // Move from T1 to T2 (frequent list)
                T1[foundT1] = -1;
                for (int k = 0; k < MAX_FRAMES; k++)
                    if (T2[k] == -1) { T2[k] = page; break; }
            }
        } else {
            misses++;
            int inserted = 0;

            // Try inserting into T1
            for (int j = 0; j < MAX_FRAMES; j++) {
                if (T1[j] == -1) {
                    T1[j] = page;
                    inserted = 1;
                    break;
                }
            }

            // If T1 full, try T2
            if (!inserted) {
                for (int j = 0; j < MAX_FRAMES; j++) {
                    if (T2[j] == -1) {
                        T2[j] = page;
                        inserted = 1;
                        break;
                    }
                }
            }

            // If both full, overwrite first in T1
            if (!inserted) {
                T1[0] = page;
            }
        }

        printf("Step %2d (Page %d): T1[", i + 1, page);
        for (int j = 0; j < MAX_FRAMES; j++) T1[j] == -1 ? printf("- ") : printf("%d ", T1[j]);
        printf("]  T2[");
        for (int j = 0; j < MAX_FRAMES; j++) T2[j] == -1 ? printf("- ") : printf("%d ", T2[j]);
        printf("] -> %s\n", (foundT1 != -1 || foundT2 != -1) ? "HIT" : "MISS");
    }

    printf("\nTotal Hits: %d\nTotal Misses: %d\nHit Ratio: %.2f\n",
           hits, misses, (float)hits / MAX_PAGES);
}

// ======================== MAIN FUNCTION ========================
int main() {
    simulateLRU();
    simulateClock();
    simulateARC();
    return 0;
}
