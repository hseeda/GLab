#include "pch.h"
#include "FileSystem.h"
#include "EventSystem.h"


namespace FS {
	bool readAbaqusInpFile(
		std::vector<float> &verts, 
		std::vector<unsigned int> &indices,
		std::string file_name)
	{
		_ps2;
		_ply(">>> Abaqus Reader Started");
		std::unordered_map<int, int> nodeMap;
		verts.clear();
		indices.clear();

		IFile f(file_name);
		int n,ii,jj,kk,ll;

		std::vector<double> dv;
		dv.reserve(10);
		std::vector<int> iv;
		iv.reserve(10);

		HTimer t;

		f.openFile();
		f.separator = ',';
		//----------------------------------------------
		int flag = 0;
		while (f.getLine())
		{
			if (f.compareLine("*node")) {
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			_pe("Error: *node not found");
			_pse2;
			return false; // *Node not found
		}
		flag = 0;
		t.pr();
		//----------------------------------------------
		int cc = 0;
		while (f.getLine())
		{
			if (getChar(f.line,0) == '*') { // node section ended
				flag = 2;
				break;
			}
			if (f.line == "") continue;
			if (f.parseLineD(dv, ',') == 4) {
				n = dv[0];
				nodeMap[n] = cc;
				verts.push_back(dv[1]);
				verts.push_back(dv[2]);
				verts.push_back(dv[3]);
				cc++;
			}
		}
		if (flag == 0) {
			_pe("Error: *element not found");
			_pse2;
			return false; // *element not found
		}
		flag = 0;
		_pl(">>> Abaqus node count: " , cc);
		_pl(">>> Verts       count: ", verts.size());
		t.pr();
		//----------------------------------------------
		f.openFile();
		while (f.getLine())
		{
			if (strCompare(f.getFirstToken() ,"*element")) {
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			_pl("Error: *element not found");
			_pse2;
			return false; // *element not found
		}
		flag = 0;
		t.pr();
		//----------------------------------------------
		while (f.getLine())
		{
			if (getChar(f.line, 0) == '*') { // node section ended
				flag = 2;
				break;
			}
			if (f.line == "") continue;
			if (f.parseLineI(iv,',') == 5) {
				
				ii = nodeMap[iv[1]];
				jj = nodeMap[iv[2]];
				kk = nodeMap[iv[3]];
				ll = nodeMap[iv[4]];
				indices.push_back(ii);
				indices.push_back(jj);
				indices.push_back(kk);

				indices.push_back(ii);
				indices.push_back(ll);
				indices.push_back(jj);

				indices.push_back(ii);
				indices.push_back(kk);
				indices.push_back(ll);

				indices.push_back(jj);
				indices.push_back(ll);
				indices.push_back(kk);

			}
		}
		if (flag == 0) return false; // *end of file reached 
		_pl(">>> Abaqus Element count: ", cc);
		_pl(">>> indeces        count: ", indices.size());
		t.pr();
		
		ES::needsUpdate();
		_pse2;
		return true;
		
	}

}
