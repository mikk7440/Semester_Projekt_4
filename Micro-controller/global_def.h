

// Color definitions
#define NO_LIGTH    0
#define GREEN       1
#define BLUE        2
#define CYAN        3
#define RED         4
#define YELLOW      5
#define MAGENTA     6
#define WHITE       7

// Time out events
#define EVENT_TO_BUTTON     1
#define EVENT_TO_LCD        2


// The different ID's for timers
#define ID_TIMER_BUTTON    1
#define ID_TIMER_QUE       2
#define ID_TIMER_SPI       3


// Button events:
#define BE_NO_EVENT         0
#define BE_SINGLE_PUSH      1
#define BE_DOUBLE_PUSH      2
#define BE_LONG_PUSH        3

// Tasks.
// ------
#define TASK_UART_RX            0
#define TASK_UART_TX            1
#define TASK_SPI_RX_0           2
#define TASK_SPI_TX_0           3
#define TASK_SPI_RX_2           4
#define TASK_SPI_TX_2           5
#define TASK_UI                 6



// Semaphores.
// -----------
#define SEM_LCD          1
#define SEM_RTC_UPDATED  2

// QUEUEs.
// -------
#define QUEUE_UART_TX   1
#define QUEUE_UART_RX   2
#define QUEUE_SPI_RX_0  3
#define QUEUE_SPI_TX_0  4
#define QUEUE_SPI_RX_2  5
#define QUEUE_SPI_TX_2  6
#define QUEUE_UI_TO_REG 7

// TRUE/FALSE
#define TRUE        1
#define FALSE       0

#define SSI_0       0
#define SSI_2       2


#define WAIT_FOREVER  0
