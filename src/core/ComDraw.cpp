#include "ComDraw.h"

using namespace std;

ComDraw::ComDraw(){}

void ComDraw::Draw_threads(string name, vector <ComThread*> threads){
	Generate_dot(name, threads);
	Generate_svg(name);
}

void ComDraw::Generate_dot(string name, vector <ComThread*> threads){
	string dot_text = "digraph links {\n";
	dot_text += "\tconcentrate=true;\n";
	map <string, vector <int> > map_in;
	map <string, vector <int> > map_out;
	int num_elem = 0;
	for(size_t i = 0; i < threads.size(); i++){
		num_elem++;
		dot_text += "\tsubgraph cluster_" + to_string(num_elem) + " {\n";
		ostringstream freq;
		freq << setprecision(8) << threads[i]->Get_freq();
		dot_text += "\t\tlabel = \"" + threads[i]->Get_name() + " - " + freq.str() + "Hz\";\n";
		dot_text += "\t\tstyle=filled;\n";
		dot_text += "\t\tcolor=lightgrey;\n";
		dot_text += "\t\tnode [style=filled,color=white];\n";
		vector <string> inputs = threads[i]->Get_inputs();
		vector <string> outputs = threads[i]->Get_outputs();
		for(size_t j = 0; j < inputs.size(); j++){
			num_elem++;
			dot_text += "\t\t" + to_string(num_elem) + " [label = \"" + inputs[j] + "\"];\n";
			map_in[inputs[j]].push_back(num_elem);
		}
		for(size_t j = 0; j < outputs.size(); j++){
			num_elem++;
			dot_text += "\t\t" + to_string(num_elem) + " [label = \"" + outputs[j] + "\"];\n";
			map_out[outputs[j]].push_back(num_elem);
		}
		dot_text += "\t}\n";
	}
	for(map <string, vector <int> >::iterator it = map_in.begin(); it != map_in.end(); ++it){
		for(size_t i = 0; i < it->second.size(); i++){
			for(size_t j = 0; j < map_out[it->first].size(); j++){
				dot_text += "\t" + to_string(map_out[it->first][j]) + "->" + to_string(it->second[i]) + " [minlen=5];\n";
			}
		}
	}
	dot_text += "}";
	ofstream out_file;
	out_file.open("./test/" + name + ".gv");
	out_file << dot_text;
	out_file.close();
}

void ComDraw::Generate_svg(string name){
	#ifdef ENABLE_GRAPHVIZ	
		FILE *dot_file = fopen(("./test/" + name + ".gv").c_str(), "r");
		FILE *svg_file = fopen(("./test/" + name + ".svg").c_str(), "w");
		GVC_t *gvc = gvContext();
		Agraph_t *g = agread(dot_file);
		gvLayout(gvc, g, "dot");
		gvRender(gvc, g, "svg", svg_file);
		gvFreeLayout(gvc, g);
		agclose(g);
		gvFreeContext(gvc);
		fclose(dot_file);
		fclose(svg_file);
	#else
		cout << "[Warning] You are trying to draw communication graph with Graphviz disabled." << endl;
	#endif
}
