#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>
#include <queue>
// Para ver los distintos sensores mirar fichero "comportamiento.hpp"

//////////////////////////////////////////////////////FUNCIONES////////////////////////////////////////////////////////////////////////////////////////////////////////

//Devuelve si una ubicación en el mapa es transitable para el agente
bool CasillaTransitable(const ubicacion &x, const vector<vector<unsigned char> > &mapa){
	return (mapa[x.f][x.c] != 'P' and mapa[x.f][x.c] != 'M' ); //Transitable si la casilla actual !=M o !=P
}

//Devuelve la ubicación siguiente según el avance del agente
ubicacion NextCasilla(const ubicacion &pos){
	ubicacion next = pos;
	switch (pos.brujula)
	{
	case norte:
		next.f = pos.f - 1;
		break;
	case noreste:
		next.f = pos.f - 1;
		next.c = pos.c + 1;
		break;
	case este:
		next.c = pos.c + 1;
		break;
	case sureste:
		next.f = pos.f + 1;
		next.c = pos.c + 1;
		break;
	case sur:
		next.f = pos.f + 1;
		break;
	case suroeste:
		next.f = pos.f + 1;
		next.c = pos.c - 1;
		break;
	case oeste:
		next.c = pos.c - 1;
		break;
	case noroeste:
		next.f = pos.f - 1;
		next.c = pos.c - 1;
		break;
	}

	return next;
}

//Devuelve el estado que se genera si el agente puede avanzar.Si no puede avanzar, devuelve como salida el mismo estado de entrada
stateN0 apply(const Action &a, const stateN0 &st, const vector<vector<unsigned char> > mapa){
	
	stateN0 st_result = st;
	ubicacion sig_ubicacion, sig_ubicacion2;
	
	switch (a){

	case act_CLB_WALK:
		st_result.ultimaOrdenColaborador = act_CLB_WALK;
		sig_ubicacion = NextCasilla(st.colaborador);
		if (CasillaTransitable(sig_ubicacion, mapa) and !(sig_ubicacion.f == st.jugador.f and sig_ubicacion.c == st.jugador.c)){
			st_result.colaborador = sig_ubicacion;
		}
	break;

	case act_CLB_TURN_SR:
		st_result.ultimaOrdenColaborador = act_CLB_TURN_SR;
		st_result.colaborador.brujula = static_cast<Orientacion>((st_result.colaborador.brujula+1)%8);
	break;

	case act_CLB_STOP:
		st_result.ultimaOrdenColaborador = act_CLB_STOP;
	break;



	case actWALK: //si prox casilla es transitable y no está ocupada por el colaborador
		sig_ubicacion = NextCasilla(st.jugador);
		if (CasillaTransitable(sig_ubicacion, mapa) and !(sig_ubicacion.f == st.colaborador.f and sig_ubicacion.c == st.colaborador.c)){
				st_result.jugador = sig_ubicacion;
			}
		if(st_result.ultimaOrdenColaborador == act_CLB_WALK) st_result  = apply(act_CLB_WALK, st_result, mapa);
		if(st_result.ultimaOrdenColaborador == act_CLB_TURN_SR) st_result = apply(act_CLB_TURN_SR, st_result, mapa);
		if(st_result.ultimaOrdenColaborador == act_CLB_STOP) st_result = apply(act_CLB_STOP, st_result, mapa);
		
	break;
	
	case actRUN: //si prox 2 casillas son transitables y no está ocupada por el colaborador
		sig_ubicacion = NextCasilla(st.jugador);
		if (CasillaTransitable(sig_ubicacion, mapa) and !(sig_ubicacion.f == st.colaborador.f and sig_ubicacion.c == st.colaborador.c)){
				sig_ubicacion2 = NextCasilla(sig_ubicacion);
				if (CasillaTransitable(sig_ubicacion2, mapa) and !(sig_ubicacion2.f == st.colaborador.f and sig_ubicacion2.c == st.colaborador.c)){
						st_result.jugador = sig_ubicacion2;
				}
		}
		if(st_result.ultimaOrdenColaborador == act_CLB_WALK) st_result    = apply(act_CLB_WALK, st_result, mapa);
		if(st_result.ultimaOrdenColaborador == act_CLB_TURN_SR) st_result = apply(act_CLB_TURN_SR, st_result, mapa);
		if(st_result.ultimaOrdenColaborador == act_CLB_STOP) st_result = apply(act_CLB_STOP, st_result, mapa);
		
	break;

	case actTURN_L:
		st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+6)%8);
		if(st_result.ultimaOrdenColaborador == act_CLB_WALK) st_result    = apply(act_CLB_WALK, st_result, mapa);
		if(st_result.ultimaOrdenColaborador == act_CLB_TURN_SR) st_result = apply(act_CLB_TURN_SR, st_result, mapa);
		if(st_result.ultimaOrdenColaborador == act_CLB_STOP) st_result = apply(act_CLB_STOP, st_result, mapa);
		
	break;

	case actTURN_SR:
		st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+1)%8);
		if(st_result.ultimaOrdenColaborador == act_CLB_WALK) st_result    = apply(act_CLB_WALK, st_result, mapa);
		if(st_result.ultimaOrdenColaborador == act_CLB_TURN_SR) st_result = apply(act_CLB_TURN_SR, st_result, mapa);
		if(st_result.ultimaOrdenColaborador == act_CLB_STOP) st_result = apply(act_CLB_STOP, st_result, mapa);
		
	break;


	case actIDLE:
		if(st_result.ultimaOrdenColaborador == act_CLB_WALK) st_result    = apply(act_CLB_WALK, st_result, mapa);
		if(st_result.ultimaOrdenColaborador == act_CLB_TURN_SR) st_result = apply(act_CLB_TURN_SR, st_result, mapa);
		if(st_result.ultimaOrdenColaborador == act_CLB_STOP) st_result = apply(act_CLB_STOP, st_result, mapa);
	break;

	}
	return st_result;
}

//Actualiza los objetos
nodeN2 actualizaObjeto(const ubicacion &pos, nodeN2 &node, const vector<vector<unsigned char> > mapa ){
	if(mapa[pos.f][pos.c] == 'K'){
			node.st.bikini = true;
			node.st.zapatillas = false;
	} else if(mapa[pos.f][pos.c] == 'D'){
			node.st.bikini = false;
			node.st.zapatillas = true;
	}
	return node;
}


//Apply para el nivel 2 
nodeN2 apply(const Action &a, const nodeN2 &node, const vector<vector<unsigned char>> mapa){
	nodeN2 node_result = node;
	ubicacion sig_ubicacion, sig_ubicacion2;
	char casilla_actual = mapa[node.st.jugador.f][node.st.jugador.c];

	switch (a){

	case actWALK: 
		sig_ubicacion = NextCasilla(node.st.jugador);
		if (CasillaTransitable(sig_ubicacion, mapa) and !(sig_ubicacion.f == node.st.colaborador.f and sig_ubicacion.c == node.st.colaborador.c)){
			
			//Actualizo ubicacion:
			node_result.st.jugador = sig_ubicacion;

			//Actualizo coste: 
			if(casilla_actual == 'B'){
				if(node.st.zapatillas) node_result.coste += 15;
				else node_result.coste += 50;
			}else if(casilla_actual == 'A'){
				if(node.st.bikini) node_result.coste += 10;
				else node_result.coste += 100;
			}else if(casilla_actual == 'T'){
				node_result.coste += 2;
			}else node_result.coste += 1;

			//Actualizo objetos (si la ubicación a la que voy tiene un objeto, ya lo actualizo): 
			actualizaObjeto(node_result.st.jugador, node_result, mapa);
		}
	break;
	
	case actRUN: 
		sig_ubicacion = NextCasilla(node.st.jugador);
		if (CasillaTransitable(sig_ubicacion, mapa) and !(sig_ubicacion.f == node.st.colaborador.f and sig_ubicacion.c == node.st.colaborador.c)){
				sig_ubicacion2 = NextCasilla(sig_ubicacion);
				if (CasillaTransitable(sig_ubicacion2, mapa) and !(sig_ubicacion2.f == node.st.colaborador.f and sig_ubicacion2.c == node.st.colaborador.c)){

						//Actualizo ubicación:
						node_result.st.jugador = sig_ubicacion2;

						//Actualizo coste: 
						if(casilla_actual == 'B'){
						if(node.st.zapatillas) node_result.coste += 25;
							else node_result.coste += 75;
						}else if(casilla_actual == 'A'){
							if(node.st.bikini) node_result.coste += 15;
							else node_result.coste += 150;
						}else if(casilla_actual == 'T'){
							node_result.coste += 3;
						}else node_result.coste += 1;

						//Actualizo objetos (si la ubicación a la que voy tiene un objeto, ya lo actualizo): 
						actualizaObjeto(node_result.st.jugador, node_result, mapa);
				}
		}
	break;

	case actTURN_L:

		//Actualizo orientación: 
		node_result.st.jugador.brujula = static_cast<Orientacion>((node_result.st.jugador.brujula+6)%8);

		//Actualizo coste: 
		if(casilla_actual == 'B'){
				if(node.st.zapatillas) node_result.coste += 1;
				else node_result.coste += 7;
			}else if(casilla_actual == 'A'){
				if(node.st.bikini) node_result.coste += 5;
				else node_result.coste += 30;
			}else if(casilla_actual == 'T'){
				node_result.coste += 2;
			}else node_result.coste += 1;
		
		
	break;

	case actTURN_SR:

		//Actualizo orientación: 	
		node_result.st.jugador.brujula = static_cast<Orientacion>((node_result.st.jugador.brujula+1)%8);

		//Actualizo coste: 
		if(casilla_actual == 'B'){
				if(node.st.zapatillas) node_result.coste += 1;
				else node_result.coste += 5;
			}else if(casilla_actual == 'A'){
				if(node.st.bikini) node_result.coste += 2;
				else node_result.coste += 10;
			}else if(casilla_actual == 'T'){
				node_result.coste += 1;
			}else node_result.coste += 1;
		
		
	break;
	}
	return node_result;
}

//Encuentra si el elemento item está en la lista de estados (stateN0)
bool Find(const stateN0 &item, const list<stateN0> &lista){

	auto it = lista.begin();
	while (it != lista.end() and !((*it) == item))
		it++;

	return (!(it == lista.end()));
}

//Sobrecarga de la función Find para buscar en lista de nodos (nodeN0)
bool Find(const stateN0 &item, const list<nodeN0> &lista){
	auto it = lista.begin();
	while (it != lista.end() and !(it->st == item))
		it++;

	return (!(it == lista.end()));
}

//Muestra en formato de texto la secuencia de acciones que constituye el plan obtenido
void PintaPlan(const list<Action> &plan){
	auto it = plan.begin();
	while (it != plan.end())
	{
		if (*it == actWALK){
			cout << "W ";
		}
		else if (*it == actRUN){
			cout << "R ";
		}
		else if (*it == actTURN_SR){
			cout << "r ";
		}
		else if (*it == actTURN_L){
			cout << "L ";
		}
		else if (*it == act_CLB_WALK){
			cout << "cW ";
		}
		else if (*it == act_CLB_TURN_SR){
			cout << "cr ";
		}
		else if (*it == act_CLB_STOP){
			cout << "cS ";
		}
		else if (*it == actIDLE){
			cout << "I ";
		}
		else{
			cout << "-_ ";
		}
		it++;
	}
	cout << " (" << plan.size() << " acciones)\n";
}

//Comprobar si el colaborador está dentro de la visión del jugador
bool CLBenVision(const stateN0 &st){
	
	int fj = st.jugador.f;
	int cj = st.jugador.c;
	int fc = st.colaborador.f;
	int cc = st.colaborador.c;

	switch(st.jugador.brujula){

		case norte:
			return (((fj-1 == fc) && (cj-1 == cc)) || ((fj-1 == fc) && (cj == cc))   || ((fj-1 == fc) && (cj+1 == cc)) ||                     
					((fj-2 == fc) && (cj-2 == cc)) || ((fj-2 == fc) && (cj-1 == cc)) || ((fj-2 == fc) && (cj == cc))   || ((fj-2 == fc) && (cj+1 == cc)) || ((fj-2 == fc) && (cj+2 == cc)) ||
					((fj-3 == fc) && (cj-3 == cc)) || ((fj-3 == fc) && (cj-2 == cc)) || ((fj-3 == fc) && (cj-1 == cc)) || ((fj-3 == fc) && (cj == cc))   || ((fj-3 == fc) && (cj+1 == cc)) || ((fj-3 == fc) && (cj+2 == cc)) || ((fj-3 == fc) && (cj+3 == cc)));
		break;
		case sur: 
			return (((fj+1 == fc) && (cj-1 == cc)) || ((fj+1 == fc) && (cj == cc))   || ((fj+1 == fc) && (cj+1 == cc)) ||                     
					((fj+2 == fc) && (cj-2 == cc)) || ((fj+2 == fc) && (cj-1 == cc)) || ((fj+2 == fc) && (cj == cc))   || ((fj+2 == fc) && (cj+1 == cc)) || ((fj+2 == fc) && (cj+2 == cc)) ||
					((fj+3 == fc) && (cj+3 == cc)) || ((fj+3 == fc) && (cj-2 == cc)) || ((fj+3 == fc) && (cj-1 == cc)) || ((fj+3 == fc) && (cj == cc))   || ((fj+3 == fc) && (cj+1 == cc)) || ((fj+3 == fc) && (cj+2 == cc)) || ((fj+3 == fc) && (cj+3 == cc)));
		break;
		case este:
			return (((fj-1 == fc) && (cj+1 == cc)) || ((fj == fc) && (cj+1 == cc))   || ((fj+1 == fc) && (cj+1 == cc)) ||                     
					((fj-2 == fc) && (cj+2 == cc)) || ((fj-1 == fc) && (cj+2 == cc)) || ((fj == fc) && (cj+2 == cc))   || ((fj+1 == fc) && (cj+2 == cc)) || ((fj+2 == fc) && (cj+2 == cc)) ||
					((fj-3 == fc) && (cj+3 == cc)) || ((fj-2 == fc) && (cj+3 == cc)) || ((fj-1 == fc) && (cj+3 == cc)) || ((fj == fc) && (cj+3 == cc))   || ((fj+1 == fc) && (cj+3 == cc)) || ((fj+2 == fc) && (cj+3 == cc)) || ((fj+3 == fc) && (cj+3 == cc)));
		break;
		case oeste:
			return (((fj-1 == fc) && (cj-1 == cc)) || ((fj == fc) && (cj-1 == cc))   || ((fj+1 == fc) && (cj-1 == cc)) ||                     
					((fj-2 == fc) && (cj-2 == cc)) || ((fj-1 == fc) && (cj-2 == cc)) || ((fj == fc) && (cj-2 == cc))   || ((fj+1 == fc) && (cj-2 == cc)) || ((fj+2 == fc) && (cj-2 == cc)) ||
					((fj-3 == fc) && (cj-3 == cc)) || ((fj-2 == fc) && (cj-3 == cc)) || ((fj-1 == fc) && (cj-3 == cc)) || ((fj == fc) && (cj-3 == cc))   || ((fj+1 == fc) && (cj-3 == cc)) || ((fj+2 == fc) && (cj-3 == cc)) || ((fj+3 == fc) && (cj-3 == cc)));
			
		break;
		case noroeste:
			return (((fj == fc) && (cj-1 == cc)) || ((fj == fc) && (cj-2 == cc))   || ((fj == fc) && (cj-3 == cc))   ||                     
					((fj-1 == fc) && (cj == cc)) || ((fj-1 == fc) && (cj-1 == cc)) || ((fj-1 == fc) && (cj-2 == cc)) || ((fj-1 == fc) && (cj-3 == cc)) || 
					((fj-2 == fc) && (cj == cc)) || ((fj-2 == fc) && (cj-1 == cc)) || ((fj-2 == fc) && (cj-2 == cc)) || ((fj-2 == fc) && (cj-3 == cc)) || 
					((fj-3 == fc) && (cj == cc)) || ((fj-3 == fc) && (cj-1 == cc)) || ((fj-3 == fc) && (cj-2 == cc)) || ((fj-3 == fc) && (cj-3 == cc)));
		break;
		case suroeste:
			return (((fj == fc) && (cj-1 == cc)) || ((fj == fc) && (cj-2 == cc))   || ((fj == fc) && (cj-3 == cc))   ||                     
					((fj+1 == fc) && (cj == cc)) || ((fj+1 == fc) && (cj-1 == cc)) || ((fj+1 == fc) && (cj-2 == cc)) || ((fj+1 == fc) && (cj-3 == cc)) || 
					((fj+2 == fc) && (cj == cc)) || ((fj+2 == fc) && (cj-1 == cc)) || ((fj+2 == fc) && (cj-2 == cc)) || ((fj+2 == fc) && (cj-3 == cc)) || 
					((fj+3 == fc) && (cj == cc)) || ((fj+3 == fc) && (cj-1 == cc)) || ((fj+3 == fc) && (cj-2 == cc)) || ((fj+3 == fc) && (cj-3 == cc)));
		break;
		case noreste:
			return (((fj == fc) && (cj+1 == cc)) || ((fj == fc) && (cj+2 == cc))   || ((fj == fc) && (cj+3 == cc))   ||                     
					((fj-1 == fc) && (cj == cc)) || ((fj-1 == fc) && (cj+1 == cc)) || ((fj-1 == fc) && (cj+2 == cc)) || ((fj-1 == fc) && (cj+3 == cc)) || 
					((fj-2 == fc) && (cj == cc)) || ((fj-2 == fc) && (cj+1 == cc)) || ((fj-2 == fc) && (cj+2 == cc)) || ((fj-2 == fc) && (cj+3 == cc)) || 
					((fj-3 == fc) && (cj == cc)) || ((fj-3 == fc) && (cj+1 == cc)) || ((fj-3 == fc) && (cj+2 == cc)) || ((fj-3 == fc) && (cj+3 == cc)));
		break;
		case sureste:
			return (((fj == fc) && (cj+1 == cc)) || ((fj == fc) && (cj+2 == cc))   || ((fj == fc) && (cj+3 == cc))   ||                     
					((fj+1 == fc) && (cj == cc)) || ((fj+1 == fc) && (cj+1 == cc)) || ((fj+1 == fc) && (cj+2 == cc)) || ((fj+1 == fc) && (cj+3 == cc)) || 
					((fj+2 == fc) && (cj == cc)) || ((fj+2 == fc) && (cj+1 == cc)) || ((fj+2 == fc) && (cj+2 == cc)) || ((fj+2 == fc) && (cj+3 == cc)) || 
					((fj+3 == fc) && (cj == cc)) || ((fj+3 == fc) && (cj+1 == cc)) || ((fj+3 == fc) && (cj+2 == cc)) || ((fj+3 == fc) && (cj+3 == cc)));
		break;
	}
}

//Primera aproximación a la implementación de la búsqueda en anchura
bool AnchuraSoloJugador(const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa){
	stateN0 current_state = inicio;
	list<stateN0> frontier;
	list<stateN0> explored;
	frontier.push_back(current_state);
	bool SolutionFound = (current_state.jugador.f == final.f and
						  current_state.jugador.c == final.c);

	while (!frontier.empty() and !SolutionFound){
		frontier.pop_front();
		explored.push_back(current_state);

		// Generar hijo actWALK
		stateN0 child_walk = apply(actWALK, current_state, mapa);
		if (child_walk.jugador.f == final.f and child_walk.jugador.c == final.c){
			current_state = child_walk;
			SolutionFound = true;
		}
		else if (!Find(child_walk, frontier) and !Find(child_walk, explored)){
			frontier.push_back(child_walk);
		}

		if (!SolutionFound){
			// Generar hijo actRUN
			stateN0 child_run = apply(actRUN, current_state, mapa);
			if (child_run.jugador.f == final.f and child_run.jugador.c == final.c){
				current_state = child_run;
				SolutionFound = true;
			}
			else if (!Find(child_run, frontier) and !Find(child_run, explored)){
				frontier.push_back(child_run);
			}
		}

		if (!SolutionFound){
			// Generar hijo actTURN_L
			stateN0 child_turnl = apply(actTURN_L, current_state, mapa);
			if (!Find(child_turnl, frontier) and !Find(child_turnl, explored)){
				frontier.push_back(child_turnl);
			}		
			// Generar hijo actTURN_SR
			stateN0 child_turnsr = apply(actTURN_SR, current_state, mapa);
			if (!Find(child_turnsr, frontier) and !Find(child_turnsr, explored)){
				frontier.push_back(child_turnsr);
			}		
		}

		if (!SolutionFound and !frontier.empty())
			current_state = frontier.front();
	}
	
	return SolutionFound;
}

//Segunda aproximación a la implementación de la búsqueda en anchura
list <Action> AnchuraSoloJugador_V2(const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa){
	
	nodeN0 current_node;
	list<nodeN0> frontier;
	list<nodeN0> explored;
	list<Action> plan;
	current_node.st = inicio;
	
	bool SolutionFound = (current_node.st.jugador.f == final.f and current_node.st.jugador.c == final.c);
	frontier.push_back(current_node);

	while (!frontier.empty() and !SolutionFound){
		frontier.pop_front();
		explored.push_back(current_node);

		// Generar hijo actWALK
		nodeN0 child_walk = current_node;
		child_walk.st = apply(actWALK, current_node.st, mapa);
		child_walk.secuencia.push_back(actWALK);
		if (child_walk.st.jugador.f == final.f and child_walk.st.jugador.c == final.c){
			current_node = child_walk;
			SolutionFound = true;
		}
		else if (!Find(child_walk.st, frontier) and !Find(child_walk.st, explored)){
			frontier.push_back(child_walk);
		}

		if (!SolutionFound){
			// Generar hijo actRUN
			nodeN0 child_run = current_node;
			child_run.st = apply(actRUN, current_node.st, mapa);
			child_run.secuencia.push_back(actRUN);
			if (child_run.st.jugador.f == final.f and child_run.st.jugador.c == final.c){
				current_node = child_run;
				SolutionFound = true;
			}
			else if (!Find(child_run.st, frontier) and !Find(child_run.st, explored)){
				frontier.push_back(child_run);
			}
		}

		if (!SolutionFound){
			// Generar hijo actTURN_L
			nodeN0 child_turnl = current_node;
			child_turnl.st = apply(actTURN_L, current_node.st, mapa);
			child_turnl.secuencia.push_back(actTURN_L);
			if (!Find(child_turnl.st, frontier) and !Find(child_turnl.st, explored)){
				frontier.push_back(child_turnl);
			}		
			// Generar hijo actTURN_SR
			nodeN0 child_turnsr = current_node;
			child_turnsr.st = apply(actTURN_SR, current_node.st, mapa);
			child_turnsr.secuencia.push_back(actTURN_SR);
			if (!Find(child_turnsr.st, frontier) and !Find(child_turnsr.st, explored)){
				frontier.push_back(child_turnsr);
			}		
		}

		if (!SolutionFound and !frontier.empty())
			current_node = frontier.front();
	}

	if(SolutionFound){
		plan = current_node.secuencia;
		cout << "Encontrado un plan: ";
		PintaPlan(current_node.secuencia);
	}
	
	return plan;
}

//ANCHURA JUGADOR
list <Action> AnchuraSoloJugador_V3(const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa){
	
	nodeN0 current_node;
	list<nodeN0> frontier;
	set<nodeN0> explored;
	list<Action> plan;
	current_node.st = inicio;
	
	bool SolutionFound = (current_node.st.jugador.f == final.f and current_node.st.jugador.c == final.c);
	frontier.push_back(current_node);

	while (!frontier.empty() and !SolutionFound){
		frontier.pop_front();
		explored.insert(current_node);


		// Generar hijo actWALK
		nodeN0 child_walk = current_node;
		child_walk.st = apply(actWALK, current_node.st, mapa);
		child_walk.secuencia.push_back(actWALK);
		if (child_walk.st.jugador.f == final.f and child_walk.st.jugador.c == final.c){
			current_node = child_walk;
			SolutionFound = true;
		}
		else if (explored.find(child_walk) == explored.end()){   // SI EL NODO NO ESTÁ EN EXPLORADOS, LO METO EN FRONTIER
			frontier.push_back(child_walk);
		}

		if (!SolutionFound){
			// Generar hijo actRUN
			nodeN0 child_run = current_node;
			child_run.st = apply(actRUN, current_node.st, mapa);
			child_run.secuencia.push_back(actRUN);
			if (child_run.st.jugador.f == final.f and child_run.st.jugador.c == final.c){
				current_node = child_run;
				SolutionFound = true;
			}
			else if (explored.find(child_run) == explored.end()){
				frontier.push_back(child_run);
			}
		}

		if (!SolutionFound){
			// Generar hijo actTURN_L
			nodeN0 child_turnl = current_node;
			child_turnl.st = apply(actTURN_L, current_node.st, mapa);
			child_turnl.secuencia.push_back(actTURN_L);
			if (explored.find(child_turnl) == explored.end()){
				frontier.push_back(child_turnl);
			}		
			// Generar hijo actTURN_SR
			nodeN0 child_turnsr = current_node;
			child_turnsr.st = apply(actTURN_SR, current_node.st, mapa);
			child_turnsr.secuencia.push_back(actTURN_SR);
			if (explored.find(child_turnsr) == explored.end()){
				frontier.push_back(child_turnsr);
			}		
		}

		if (!SolutionFound and !frontier.empty()){
			current_node = frontier.front();
		 	while(!frontier.empty() and explored.find(current_node) != explored.end()){
				frontier.pop_front();
				if(!frontier.empty())
					current_node = frontier.front();
			}
		}
			
	}

	if(SolutionFound){
		plan = current_node.secuencia;
		cout << "Encontrado un plan: ";
		PintaPlan(current_node.secuencia);
	}
	
	return plan;
}

//ANCHURA COLABORADOR
list <Action> AnchuraColaborador(const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa){
	nodeN0 current_node;
	list<nodeN0> frontier;
	set<nodeN0> explored;
	list<Action> plan;
	current_node.st = inicio;

	bool SolutionFound = (current_node.st.colaborador.f == final.f and current_node.st.colaborador.c == final.c);
	frontier.push_back(current_node);

	while (!frontier.empty() and !SolutionFound){
		frontier.pop_front();
		explored.insert(current_node);

		if(CLBenVision(current_node.st)){
	
				//Generar hijo act_CLB_walk
				nodeN0 child_clbwalk = current_node;
				child_clbwalk.st = apply(act_CLB_WALK, current_node.st, mapa);
				child_clbwalk.secuencia.push_back(act_CLB_WALK);
				if(child_clbwalk.st.colaborador.f == final.f and child_clbwalk.st.colaborador.c == final.c){
					current_node = child_clbwalk;
					SolutionFound = true;
				}else if (explored.find(child_clbwalk) == explored.end()){
							frontier.push_back(child_clbwalk);
						}
			
			
			
				if(!SolutionFound && current_node.st.ultimaOrdenColaborador != act_CLB_TURN_SR){
				//Generar hijo act_CLB_TURN_SR
				nodeN0 child_clbturnsr = current_node;
				child_clbturnsr.st = apply(act_CLB_TURN_SR, current_node.st, mapa);
				child_clbturnsr.secuencia.push_back(act_CLB_TURN_SR);
				if (explored.find(child_clbturnsr) == explored.end()){
					frontier.push_back(child_clbturnsr);
				}	
				}
			
				if(!SolutionFound && current_node.st.ultimaOrdenColaborador != act_CLB_STOP){
					//Generar hijo act_CLB-STOP
					nodeN0 child_clbstop = current_node;
					child_clbstop.st = apply(act_CLB_STOP, current_node.st, mapa);
					child_clbstop.secuencia.push_back(act_CLB_STOP);
					if (explored.find(child_clbstop) == explored.end()){
					frontier.push_back(child_clbstop);
					}	
				}
		}


		if(!SolutionFound){
			// Generar hijo actWALK
			nodeN0 child_walk = current_node;
			child_walk.st = apply(actWALK, current_node.st, mapa);
			child_walk.secuencia.push_back(actWALK);
			if(current_node.st.colaborador.f == final.f and current_node.st.colaborador.c == final.c){
					SolutionFound = true;
			}else if(explored.find(child_walk) == explored.end()){   // SI EL NODO NO ESTÁ EN EXPLORADOS, LO METO EN FRONTIER
						frontier.push_back(child_walk);
			}
		}

		if(!SolutionFound){
			// Generar hijo actRUN
			nodeN0 child_run = current_node;
			child_run.st = apply(actRUN, current_node.st, mapa);
			child_run.secuencia.push_back(actRUN);
			if(current_node.st.colaborador.f == final.f and current_node.st.colaborador.c == final.c){
					SolutionFound = true;
			}else if (explored.find(child_run) == explored.end()){
						frontier.push_back(child_run);
			}
		}
		

		if(!SolutionFound){
			// Generar hijo actTURN_L
			nodeN0 child_turnl = current_node;
			child_turnl.st = apply(actTURN_L, current_node.st, mapa);
			child_turnl.secuencia.push_back(actTURN_L);
			if(current_node.st.colaborador.f == final.f and current_node.st.colaborador.c == final.c){
					SolutionFound = true;
			}else if (explored.find(child_turnl) == explored.end()){
				frontier.push_back(child_turnl);
			}
		}

		if(!SolutionFound){
			// Generar hijo actTURN_SR
			nodeN0 child_turnsr = current_node;
			child_turnsr.st = apply(actTURN_SR, current_node.st, mapa);
			child_turnsr.secuencia.push_back(actTURN_SR);
			if(current_node.st.colaborador.f == final.f and current_node.st.colaborador.c == final.c){
					SolutionFound = true;
			}else if (explored.find(child_turnsr) == explored.end()){
				frontier.push_back(child_turnsr);
			}
		}

		if(!SolutionFound){
			//Generar hijo actIDLE
			nodeN0 child_idle = current_node;
			child_idle.st = apply(actIDLE, current_node.st, mapa);
			child_idle.secuencia.push_back(actIDLE);
			if(current_node.st.colaborador.f == final.f and current_node.st.colaborador.c == final.c){
					SolutionFound = true;
			}else if (explored.find(child_idle) == explored.end()){
				frontier.push_back(child_idle);
			}
		}
	
		if (!SolutionFound and !frontier.empty()){
			current_node = frontier.front();
		 	while(!frontier.empty() and explored.find(current_node) != explored.end()){
				frontier.pop_front();
				if(!frontier.empty()) current_node = frontier.front();
			}
		}
	}

	if(SolutionFound){
		plan = current_node.secuencia;
		cout << "Encontrado un plan: ";
		PintaPlan(current_node.secuencia);
	}
	return plan;
}
	
list <Action> DijkstraJugador(const stateN2 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa){
	
	nodeN2 current_node;
	priority_queue <nodeN2> frontier;
	set<stateN2> explored;
	list<Action> plan;
	current_node.st = inicio;


	bool SolutionFound = (current_node.st.jugador.f == final.f and current_node.st.jugador.c == final.c);
	frontier.push(current_node);

	if(mapa[current_node.st.jugador.f][current_node.st.jugador.c] == 'K'){
				current_node.st.bikini = true;
				current_node.st.zapatillas = false;
			} else if(mapa[current_node.st.jugador.f][current_node.st.jugador.c] == 'D'){
				current_node.st.bikini = false;
				current_node.st.zapatillas = true;
			}

	while (!frontier.empty() and !SolutionFound){
		
		//Coge el nodo con menor coste lo inserta en explorados y lo elimina de abiertos (frontier)
		//current_node = frontier.top();  //REVISAR
        //frontier.pop();

		frontier.pop();
		explored.insert(current_node.st); 

		if(current_node.st.jugador.f == final.f and current_node.st.jugador.c == final.c){
			SolutionFound = true;
		}else{
			SolutionFound = false;
		}


		if(!SolutionFound){
			
			// Generar hijo actWALK
			nodeN2 child_walk = current_node;
			child_walk = apply(actWALK, current_node, mapa);
			child_walk.secuencia.push_back(actWALK);
			if(explored.find(child_walk.st) == explored.end()){   // SI EL NODO NO ESTÁ EN EXPLORADOS, LO METO EN FRONTIER
						frontier.push(child_walk);
			}
		

			// Generar hijo actRUN
			nodeN2 child_run = current_node;
			child_run= apply(actRUN, current_node, mapa);
			child_run.secuencia.push_back(actRUN);
			if (explored.find(child_run.st) == explored.end()){
						frontier.push(child_run);
			}
		
			// Generar hijo actTURN_L
			nodeN2 child_turnl = current_node;
			child_turnl = apply(actTURN_L, current_node, mapa);
			child_turnl.secuencia.push_back(actTURN_L);
			if (explored.find(child_turnl.st) == explored.end())
				frontier.push(child_turnl);
			
		
			// Generar hijo actTURN_SR
			nodeN2 child_turnsr = current_node;
			child_turnsr = apply(actTURN_SR, current_node, mapa);
			child_turnsr.secuencia.push_back(actTURN_SR);
			if (explored.find(child_turnsr.st) == explored.end())
				frontier.push(child_turnsr);
				

		}

		/*if (!SolutionFound and !frontier.empty()){
			current_node = frontier.top();
			if(current_node.st.jugador.f == final.f and current_node.st.jugador.c == final.c){
				SolutionFound = true;
			}
		}*/

		if (!SolutionFound and !frontier.empty()){
		current_node = frontier.top();
		 	while(!frontier.empty() and explored.find(current_node.st) != explored.end()){
				frontier.pop();
				if(!frontier.empty()){
					current_node = frontier.top();
					//frontier.pop(); //REVISAR
				} 
			}
		}

		
	}

	if(SolutionFound){
		plan = current_node.secuencia;
		cout << "Encontrado un plan: ";
		PintaPlan(current_node.secuencia);
	}
	return plan;
}

//Pone a cero todos los elementos de una matriz
void AnulaMatriz(vector<vector<unsigned char> > &matriz){
	for (int i = 0; i < matriz.size(); i++)
		for (int j = 0; j < matriz[0].size(); j++)
			matriz[i][j] = 0;
}

//Permite pintar sobre el mapa del simulador el plan partiendo desde el estado st*/
void ComportamientoJugador::VisualizaPlan(const stateN0 &st, const list<Action> &plan){
	AnulaMatriz(mapaConPlan);
	stateN0 cst = st;

	auto it = plan.begin();
	while (it != plan.end())
	{
		if ((*it != act_CLB_WALK) and (*it != act_CLB_TURN_SR) and (*it != act_CLB_STOP))
		{
			switch (cst.ultimaOrdenColaborador)
			{
			case act_CLB_WALK:
				cst.colaborador = NextCasilla(cst.colaborador);
				mapaConPlan[cst.colaborador.f][cst.colaborador.c] = 2;
				break;
			case act_CLB_TURN_SR:
				cst.colaborador.brujula = (Orientacion)((cst.colaborador.brujula + 1) % 8);
				break;
			}
		}

		switch (*it)
		{
		case actRUN:
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 3;
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actWALK:
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actTURN_SR:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 1) % 8);
			break;
		case actTURN_L:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 6) % 8);
			break;
		case act_CLB_WALK:
			cst.colaborador = NextCasilla(cst.colaborador);
			cst.ultimaOrdenColaborador = act_CLB_WALK;
			mapaConPlan[cst.colaborador.f][cst.colaborador.c] = 2;
			break;
		case act_CLB_TURN_SR:
			cst.colaborador.brujula = (Orientacion)((cst.colaborador.brujula + 1) % 8);
			cst.ultimaOrdenColaborador = act_CLB_TURN_SR;
			break;
		case act_CLB_STOP:
			cst.ultimaOrdenColaborador = act_CLB_STOP;
			break;
		}
		it++;
	}
}


//////////////////////////////////////////////////////FUNCION PRINCIPAL////////////////////////////////////////////////////////////////////////////////////////////////

Action ComportamientoJugador::think(Sensores sensores)
{
	Action accion = actIDLE;
	
	if(sensores.nivel !=4){
		
		if (!hayPlan){
			cout << "Calculamos un nuevo plan\n";
			//Inicializar nodo Nivel 1 y 2: 
			c_state.jugador.f = sensores.posF;
			c_state.jugador.c = sensores.posC;
			c_state.jugador.brujula = sensores.sentido;
			c_state.colaborador.f = sensores.CLBposF;
			c_state.colaborador.c = sensores.CLBposC;
			c_state.colaborador.brujula = sensores.CLBsentido;
			c_state.ultimaOrdenColaborador = act_CLB_STOP;

			//INICIALIZAR nodo Nivel 2: 
			c_node.st.jugador.f = sensores.posF;
			c_node.st.jugador.c = sensores.posC;
			c_node.st.colaborador.f = sensores.CLBposF;
			c_node.st.colaborador.c = sensores.CLBposC;
			c_node.st.jugador.brujula = sensores.sentido;
			c_node.st.colaborador.brujula = sensores.CLBsentido;  //No pongo la última orden del colaborador porque no nos interesa (La ubicación si para que no se chique el jugador)
			c_node.st.bikini = false;
			c_node.st.zapatillas = false;
			c_node.coste = 0;

			goal.f = sensores.destinoF;
			goal.c = sensores.destinoC;

			switch (sensores.nivel){
				case 0: plan = AnchuraSoloJugador_V3(c_state, goal, mapaResultado);
					break;
				case 1: plan = AnchuraColaborador(c_state, goal, mapaResultado);
					break;
				case 2: plan = DijkstraJugador(c_node.st, goal, mapaResultado);
					break;
				case 3: 
					//Incluir aquí la llamada al alg. búsqueda del nivel 3
					break;
			}
			
			if(plan.size()>0){
				VisualizaPlan(c_state,plan);
				hayPlan = true;
			}	
		}
		
		if (hayPlan and plan.size()>0){
			accion = plan.front();
			plan.pop_front();
		}
		
		if (plan.size()== 0){
			cout << "Se completó el plan\n";
			hayPlan = false;
		}

	}else{
		////Incluir aquí la solución al nivel 4
	}
	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}