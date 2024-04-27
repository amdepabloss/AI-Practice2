#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
#include <list>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct stateN0{
  ubicacion jugador;
  ubicacion colaborador;
  Action ultimaOrdenColaborador; //Relevante para los niveles que implican movimiento del agente colaborador

  //Dos estados son iguales si la ubicación del jugador y la posición del colaborador, son iguales en ambos estados.
  bool operator==(const stateN0 &x)const{
    if(jugador == x.jugador and
       colaborador.f == x.colaborador.f and colaborador.c == x.colaborador.c and ultimaOrdenColaborador == x.ultimaOrdenColaborador){ //modificado
        return true;
    }
    else {
      return false;
    }
  }
};

//Definición del tipo nodo del nivel 0
struct nodeN0{
	stateN0 st;
	list<Action> secuencia;

	bool operator==(const nodeN0 &n) const {
		return (st == n.st);
	}

	bool operator<(const nodeN0 &b)  const {
		return  (st.jugador.f < b.st.jugador.f) || (st.jugador.f == b.st.jugador.f and st.jugador.c < b.st.jugador.c) || (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and st.jugador.brujula < b.st.jugador.brujula) ||
    (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and st.jugador.brujula == b.st.jugador.brujula and st.colaborador.f < b.st.colaborador.f) ||
    (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and st.jugador.brujula == b.st.jugador.brujula and st.colaborador.f ==b.st.colaborador.f and st.colaborador.c < b.st.colaborador.c) ||
    (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and st.jugador.brujula == b.st.jugador.brujula and st.colaborador.f ==b.st.colaborador.f and st.colaborador.c == b.st.colaborador.c and st.colaborador.brujula < b.st.colaborador.brujula) ||
    (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and st.jugador.brujula == b.st.jugador.brujula and st.colaborador.f ==b.st.colaborador.f and st.colaborador.c == b.st.colaborador.c and st.colaborador.brujula == b.st.colaborador.brujula and st.ultimaOrdenColaborador < b.st.ultimaOrdenColaborador) ;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class ComportamientoJugador : public Comportamiento {
  public:

    // Inicializar Variables de Estado nivel 4
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
      
    }

    // Inicializar Variables de Estado niveles 0-3
    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
      hayPlan = false;
    }
    
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    void VisualizaPlan(const stateN0 &st, const list<Action> &plan);

    Action think(Sensores sensores);
    int interact(Action accion, int valor);


  private:
    list<Action> plan; //Almacena el plan en ejecución
    bool hayPlan;      //Si es true indica que se está siguiendo un plan
    stateN0 c_state;
    ubicacion goal;
};

#endif
