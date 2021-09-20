#include "abstractthread.h"

/**
 * @brief AbstractThread::AbstractThread
 *
 * Constructeur de thread
 *
 * Description d'un thread
 * -----------------------
 *
 * Classe permettant de représenter des fils d'exécution individuels.
 * Un thread d'exécution est une séquence d'instructions qui peut être exécutée
 * simultanément avec d'autres séquences, tout en partageant un même espace d'adressage.
 *
 * Un objet thread initialisé représente un thread d'exécution actif ;
 * un tel objet thread est joignable et possède un identifiant de thread unique.
 *
 * Un objet thread construit par défaut (non initialisé) n'est pas joignable,
 * et son identifiant est commun à tous les threads non joignables.
 *
 * Un thread joignable devient non joignable s'il est déplacé, ou si join ou detach sont appelés sur lui.
 */
AbstractThread::AbstractThread() : thread(), started(false), terminating(false){
    thread = std::move(std::thread(&AbstractThread::abstract_thread_main, std::ref(*this)));
}


/**
 * @brief AbstractThread::start
 *
 * Crée un mutex. Un mutex permet la synchronisation des données
 * que l'on utilise pour protéger les données partagées contre un accès simultané par plusieurs threads.
 *
 */
void AbstractThread::start() {
    std::unique_lock<std::mutex> lck(mutex);
    started = true;
    lck.unlock();    // Déverrouille le mutex, libérant la propriété sur celui-ci

    cv.notify_one(); // Changement d'état: en attente -> en marche
}


/**
 * @brief AbstractThread::isRunning
 * @return
 *
 * Renvoie la valeur de started
 */
bool AbstractThread::isRunning() {return started;}


/**
 * @brief AbstractThread::isFinished
 * @return
 *
 * Renvoie la valeur de !started
*/
bool AbstractThread::isFinished() {return !started;}


/**
 * @brief AbstractThread::waitForFinish
 *
 * Fonction pour déverrouiller le mutex lors de son utilisation
 */
void AbstractThread::waitForFinish(){
    while (isRunning()) {
        std::unique_lock<std::mutex> lck(mutex);
        lck.unlock();         // Déverrouille le mutex
    }
}


/**
 * @brief AbstractThread::~AbstractThread
 *
 * Destructeur du thread
 */
AbstractThread::~AbstractThread() {
    std::unique_lock<std::mutex> lck(mutex);
    terminating = true;
    lck.unlock();              // Déverrouille le mutex
    cv.notify_one();           // Débloque le thread actuellement en attente de cette condition
    if (thread.joinable())     // Vérifie si l'objet le thread identifie un thread d'exécution actif.
        thread.join();         // Bloque le thread appelant jusqu’à ce que le thread représenté par cette instance s’arrête
}


/**
 * @brief AbstractThread::abstract_thread_main
 *
 * Fonction du gestionnaire du thread
 */
void AbstractThread::abstract_thread_main() {
    while (true) {
        std::unique_lock<std::mutex> lck(mutex);

        // attendre l'appel start()
        cv.wait(lck, [this](){return started || terminating;}); // Changement d'état: en attente -> en marche

        if (terminating)    // Termine le thread
            return;

        run();
        started = false;    // changement d'état: en marche -> en attente
    }
}

