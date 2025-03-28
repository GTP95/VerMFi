
////////////////////////////////////////////////////////////////////////////////

// COMPANY:		COSIC, KU Leuven 
// AUTHOR:		Victor Arribas

////////////////////////////////////////////////////////////////////////////////

// BSD 3-Clause License

// Copyright (c) 2019, Victor Arribas
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.

// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

////////////////////////////////////////////////////////////////////////////////

#include "VerifTool.h"

void Netlist::subcircuit_search_back(Wire *w, Subcircuit *subcirc, wires &remaining_outs){

	//If it is the first entry, it means is an output... store it
	// cout << "Getting in search back subcirc! wire: " << w->name_  << std::endl;
	if ((outsMap_.find(w->name_) != outsMap_.end()) || (newOutputsMap_.find(w->name_) != newOutputsMap_.end()))	
	{
		//The wire is an output. Include it in the sub_outs and erase it from remaining_outs
		//Save the wire if not saved already
		if (std::find(remaining_outs.begin(), remaining_outs.end(), w) != remaining_outs.end())
		{
			// cout << "Saving output: " << w->name_ << std::endl;
			subcirc->subcircuitOutputs_.push_back(w);
			remaining_outs.erase(std::find(remaining_outs.begin(), remaining_outs.end(), w));
		}	
	}

	if ((inputsMap_.find(w->name_) != inputsMap_.end()) || (newInputsMap_.find(w->name_) != newInputsMap_.end()) || (w->name_ == "one") or (w->name_ == "zero") or (w->get_is_PI()))	
	{
		//The wire is an input, a new input, a zero or one, or a random var input. Save it in the subcircuit inputs vector and go forth from that input
		if (std::find(subcirc->subcircuitInputs_.begin(), subcirc->subcircuitInputs_.end(), w) == subcirc->subcircuitInputs_.end()){
			// cout << "Saving input dep: " << w->name_ << std::endl;
			subcirc->subcircuitInputs_.push_back(w);
			subcircuit_search_forth(w, subcirc, remaining_outs);
			// cout << "Back to wire11: " << w->name_ << endl;
		}
	}else if ((randomsMap_.find(w->name_) != randomsMap_.end()))	
	{
		//The wire is an input, a new input, a zero or one, or a random var input. Save it in the subcircuit inputs vector and go forth from that input
		if (std::find(subcirc->subcircuitRandom_.begin(), subcirc->subcircuitRandom_.end(), w) == subcirc->subcircuitRandom_.end()){
			// cout << "Saving random: " << w->name_ << std::endl;
			subcirc->subcircuitRandom_.push_back(w);
			subcircuit_search_forth(w, subcirc, remaining_outs);
			// cout << "Back to wire22: " << w->name_ << endl;
		}
	}
	else{
		//Continue back
		//Save first the current gate driving this wireif it does not exist already
		if (std::find(subcirc->subcircuitGates_.begin(), subcirc->subcircuitGates_.end(), w->pinFrom_->gate_) == subcirc->subcircuitGates_.end())
		{
			// cout << "Saving gate: " << w->pinFrom_->gate_->gateName_ << std::endl;
			assert(w->pinFrom_->gate_->get_type() != Gate::DFF && "Saving a register into a combinational subcircuit");
			subcirc->subcircuitGates_.push_back(w->pinFrom_->gate_);
		}
		
		//Apply this same function to every input wire of the gate driving this wire
		// cout << "Number of inputs for wire: " << w->name_ << endl;
		// cout << "	" << w->pinFrom_->gate_->inputs_.size() << std::endl;
		for (size_t i = 0; i < w->pinFrom_->gate_->inputs_.size(); ++i)
		{
			// cout << "Comming from pin: " << w->pinFrom_->gate_->inputs_[i]->get_name() << endl;
			// cout << "Recursive function back goes to wire: " << w->pinFrom_->gate_->inputs_[i]->wire_->name_ << "	from wire: " << w->name_ << endl;
			subcircuit_search_back(w->pinFrom_->gate_->inputs_[i]->wire_, subcirc, remaining_outs);
		}
		// cout << "Back to wire33: " << w->name_ << endl;
	}

	// cout << "Finished back: " << w->name_ << endl;
		
}