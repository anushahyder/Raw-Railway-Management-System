#include "globals.h"

int seatsUsed(int si)
{
    int r, c, n = 0;
    for (r = 0; r < SEAT_ROWS; r++)
        for (c = 0; c < SEAT_COLS; c++)
            if (schedules[si].seats[r][c] != 0) n++;
    return n;
}

int availableSeats(int si)
{
    return trains[schedules[si].trainIndex].totalSeats - seatsUsed(si);
}

void showSeatMap(int si)
{
    int r, c, n;
    struct Train *t = &trains[schedules[si].trainIndex];
    printf("\n  Seat map: %s | %s %s\n", t->name, schedules[si].date, schedules[si].time);
    printf("        ");
    for (c = 0; c < SEAT_COLS; c++) printf("%3d", c + 1);
    printf("\n");
    for (r = 0; r < SEAT_ROWS; r++) {
        printf("  Row %2d", r + 1);
        for (c = 0; c < SEAT_COLS; c++) {
            n = r * SEAT_COLS + c;
            if (n >= t->totalSeats) printf("%3s", "-");
            else printf("%3s", schedules[si].seats[r][c] == 0 ? "O" : "X");
        }
        printf("\n");
    }
    printf("  O = Available, X = Booked, - = No seat\n");
}

void seatMapMenu(void)
{
    int id, i, si = -1;
    displaySchedules();
    id = readInt("\n  Enter Schedule ID for seat map (0 = back): ");
    if (id == 0) return;
    for (i = 0; i < scheduleCount; i++)
        if (schedules[i].active && schedules[i].id == id) { si = i; break; }
    if (si == -1) { printf("  Schedule not found.\n"); return; }
    showSeatMap(si);
}

int chooseSeat(int si, int *seatRow, int *seatCol)
{
    int ti = schedules[si].trainIndex;
    int row, col, r, c;

    showSeatMap(si);
    row = readInt("  Choose seat row (0 = auto-assign): ");
    if (row == 0) {
        for (r = 0; r < SEAT_ROWS; r++)
            for (c = 0; c < SEAT_COLS; c++)
                if (r * SEAT_COLS + c < trains[ti].totalSeats && schedules[si].seats[r][c] == 0) {
                    *seatRow = r; *seatCol = c;
                    return 1;
                }
        printf("  No seat available.\n");
        return 0;
    }
    col = readInt("  Choose seat column: ");
    r = row - 1; c = col - 1;
    if (r < 0 || r >= SEAT_ROWS || c < 0 || c >= SEAT_COLS ||  r * SEAT_COLS + c >= trains[ti].totalSeats)
    { printf("  [!] That seat does not exist.\n");
     return 0; }
    if (schedules[si].seats[r][c] != 0)
    { printf("  [!] Seat already booked.\n"); 
     return 0; }
    *seatRow = r; *seatCol = c;
    return 1;
}

void bookSeat(int si, int row, int col, int bookingId)
{
    schedules[si].seats[row][col] = bookingId;
}

void releaseSeat(int si, int row, int col)
{
    if (si >= 0 && si < scheduleCount && row >= 0 && row < SEAT_ROWS && col >= 0 && col < SEAT_COLS)
        schedules[si].seats[row][col] = 0;
}
