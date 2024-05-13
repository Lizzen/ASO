#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <mqueue.h>

// Manejador de errores de forma consistente
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

// Función ejecutada por cada thread para manejar las solicitudes de los clientesg
// Recibe un puntero a un descriptor de la cola de mensajes. Lo procesa y envia una respuesta al cliente
static void *thread_func(void *arg) {
    mqd_t mqdes = *((mqd_t *) arg);
    struct mq_attr attr;
    ssize_t nr;
    void *buf;

    if (mq_getattr(mqdes, &attr) == -1)
        handle_error("mq_getattr");

    buf = malloc(attr.mq_msgsize);
    if (buf == NULL)
        handle_error("malloc");

    nr = mq_receive(mqdes, buf, attr.mq_msgsize, NULL);
    if (nr == -1)
        handle_error("mq_receive");

    // Procesa la solicitud del cliente y enviar la respuesta

    printf("Read %zd bytes from MQ\n", nr);

    free(buf);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    mqd_t mqdes;                    // almacena el descriptor de la cola de mensajes
    struct mq_attr mq_attr;         // atributos de la cola de mensajes
    struct sigevent sev;
    pthread_t thread;
    int status;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <mq-name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Crea ls cola de mensajes POSIX
    mq_attr.mq_flags = 0;           // flag de los mensajes
    mq_attr.mq_maxmsg = 10;         // Máximo 10 mensajes en la cola
    mq_attr.mq_msgsize = 1024;      // Tamaño máximo del mensaje
    mq_attr.mq_curmsgs = 0;         // Ní,ero actual de mensajes de la cola

    // Crea la cola con los atributos
    mqdes = mq_open(argv[1], O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR, &mq_attr);
    if (mqdes == (mqd_t) -1){
        handle_error("mq_open: Error al crear la cola de mensajes.\n");
    }
    // Configura la notificación de mensajes
    sev.sigev_notify = SIGEV_THREAD;            // Crea un nuevo thread para manejar los nuevos mensajes de la cola
    sev.sigev_notify_function = thread_func;
    sev.sigev_notify_attributes = NULL;
    sev.sigev_value.sival_ptr = &mqdes;

    if (mq_notify(mqdes, &sev) == -1){
        handle_error("mq_notify: Error al configurar las notificacione de mensajes.\n");
    }

    // Espera a que lleguen mensajes. Permote que el proceso principal permanezca activo y pueda recibir notificaciones
    // El proceso será terminado por la función del thread
    pause(); 

    // Cierra la cola de mensajes
    if (mq_close(mqdes) == -1){
        handle_error("mq_close: Error al cerrar la cola de mensajes.\n");
    }
    // Elimina la cola de mensajes
    if (mq_unlink(argv[1]) == -1)
        handle_error("mq_unlink: Error al eliminar la cola de mensajes.\n");

    exit(EXIT_SUCCESS);
}
