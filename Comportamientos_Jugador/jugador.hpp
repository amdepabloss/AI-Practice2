#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
#include <list>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Definición del tipo estado del nivel 0 y 1
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

//Definición del tipo nodo del nivel 0 y 1
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

//Definición del tipo estado del nivel 2
struct stateN2{
  ubicacion jugador, colaborador; //He dejado la ubicacion del colaborador para el apply ya que necesito saber donde está para no chocarse
  bool bikini, zapatillas;

  bool operator==(const stateN2 &x)const{
    if(jugador == x.jugador and bikini == x.bikini and zapatillas == x.zapatillas){  //El coste no lo comparo ya que pueden haber 2 nodos que sean el mismo pero tienen coste diferente porque la forma de llegar a ellos es diferente.
        return true;
    }
    else {
      return false;
    }
  }

  bool operator<(const stateN2 &estado)const {
    return (jugador.f < estado.jugador.f) || (jugador.f == estado.jugador.f and jugador.c < estado.jugador.c) || (jugador.f == estado.jugador.f and jugador.c == estado.jugador.c and jugador.brujula < estado.jugador.brujula) || 
           (jugador.f == estado.jugador.f and jugador.c == estado.jugador.c and jugador.brujula == estado.jugador.brujula and bikini < estado.bikini) ||
           (jugador.f == estado.jugador.f and jugador.c == estado.jugador.c and jugador.brujula == estado.jugador.brujula and bikini == estado.bikini and zapatillas < estado.zapatillas);
           
  }
};

//Definición del tipo nodo del nivel 2
struct nodeN2{
	stateN2 st;
	list<Action> secuencia;
  int coste;
  
	bool operator==(const nodeN2 &n) const {
		return (st == n.st);
	}

	bool operator<(const nodeN2 &b) const {
		return (coste > b.coste);          
  }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Definición del tipo estado del nivel 3
struct stateN3{
  ubicacion jugador, colaborador; 
  bool bikini_colab, zapatillas_colab, bikini_jug, zapatillas_jug;
  Action ultimaOrdenColaborador;                                                                                                                                                                    

  bool operator==(const stateN3 &x)const{                                                         
    if(jugador == x.jugador and colaborador.f == x.colaborador.f and colaborador.c == x.colaborador.c and ultimaOrdenColaborador == x.ultimaOrdenColaborador and
      bikini_jug == x.bikini_jug and zapatillas_jug == x.zapatillas_jug and bikini_colab == x.bikini_colab and zapatillas_colab == x.zapatillas_colab){  
        return true;
    }
    else {
      return false;
    }
  }

  
  bool operator<(const stateN3 &b)const {
    return (jugador.f < b.jugador.f) || 
           (jugador.f == b.jugador.f and jugador.c < b.jugador.c) || 
           (jugador.f == b.jugador.f and jugador.c == b.jugador.c and jugador.brujula < b.jugador.brujula) ||
           (jugador.f == b.jugador.f and jugador.c == b.jugador.c and jugador.brujula == b.jugador.brujula and colaborador.f < b.colaborador.f) ||
           (jugador.f == b.jugador.f and jugador.c == b.jugador.c and jugador.brujula == b.jugador.brujula and colaborador.f ==b.colaborador.f and colaborador.c < b.colaborador.c) ||
           (jugador.f == b.jugador.f and jugador.c == b.jugador.c and jugador.brujula == b.jugador.brujula and colaborador.f ==b.colaborador.f and colaborador.c == b.colaborador.c and colaborador.brujula < b.colaborador.brujula) ||
           (jugador.f == b.jugador.f and jugador.c == b.jugador.c and jugador.brujula == b.jugador.brujula and colaborador.f ==b.colaborador.f and colaborador.c == b.colaborador.c and colaborador.brujula == b.colaborador.brujula and ultimaOrdenColaborador < b.ultimaOrdenColaborador) ||
           (jugador.f == b.jugador.f and jugador.c == b.jugador.c and jugador.brujula == b.jugador.brujula and colaborador.f ==b.colaborador.f and colaborador.c == b.colaborador.c and colaborador.brujula == b.colaborador.brujula and ultimaOrdenColaborador == b.ultimaOrdenColaborador and bikini_jug < b.bikini_jug) || 
           (jugador.f == b.jugador.f and jugador.c == b.jugador.c and jugador.brujula == b.jugador.brujula and colaborador.f ==b.colaborador.f and colaborador.c == b.colaborador.c and colaborador.brujula == b.colaborador.brujula and ultimaOrdenColaborador == b.ultimaOrdenColaborador and bikini_jug == b.bikini_jug and zapatillas_jug < b.zapatillas_jug) ||
           (jugador.f == b.jugador.f and jugador.c == b.jugador.c and jugador.brujula == b.jugador.brujula and colaborador.f ==b.colaborador.f and colaborador.c == b.colaborador.c and colaborador.brujula == b.colaborador.brujula and ultimaOrdenColaborador == b.ultimaOrdenColaborador and bikini_jug == b.bikini_jug and zapatillas_jug == b.zapatillas_jug and bikini_colab < b.bikini_colab) ||
           (jugador.f == b.jugador.f and jugador.c == b.jugador.c and jugador.brujula == b.jugador.brujula and colaborador.f ==b.colaborador.f and colaborador.c == b.colaborador.c and colaborador.brujula == b.colaborador.brujula and ultimaOrdenColaborador == b.ultimaOrdenColaborador and bikini_jug == b.bikini_jug and zapatillas_jug == b.zapatillas_jug and bikini_colab == b.bikini_colab and zapatillas_colab < b.zapatillas_colab);
  }
};

//Definición del tipo nodo del nivel 3
struct nodeN3{
	stateN3 st;
	list<Action> secuencia;
  int coste;
  int heuristica;   
  
	bool operator==(const nodeN3 &n) const {
		return (st == n.st);
	}

	bool operator<(const nodeN3 &b) const {  
		return ((coste+heuristica) > (b.coste+b.heuristica));          
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
    nodeN2 c_node;
    nodeN3 c_nodeN3;
    ubicacion goal;
};

#endif