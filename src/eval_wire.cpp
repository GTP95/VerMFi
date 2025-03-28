
////////////////////////////////////////////////////////////////////////////////

// COMPANY:   COSIC, KU Leuven 
// AUTHOR:    Victor Arribas

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


void Wire::eval_wire(bool val, gates& eventList){

  if ((!evaluated_) || (val != value_)){
    value_ = val;
    evaluated_ = 1;

    //Triger next event
    if (this->pinTo_ != NULL)
    {
      //Check if the gate is already stored. If it is do not save it again
      if (std::find(eventList.begin(), eventList.end(), this->pinTo_->get_gate()) == eventList.end())
      {
        // std::cout << "Saving gate " << this->pinTo->gate->gateName_ << " to eventList" << std::endl;
        eventList.push_back(this->pinTo_->get_gate());
      }
    }
    // If it is a fanOut trigger rest of the gates as well
    if (this->fanOut_)
    {
      for (size_t i = 0; i < fanOutPins_.size(); ++i)
      {
        //Check if the gate is already stored. If it is do not save it again
        if (std::find(eventList.begin(), eventList.end(), this->fanOutPins_[i]->get_gate()) == eventList.end())
        {
          // std::cout << "Saving gate " << this->fanOutPins[i]->gate->gateName_ << " to eventList" << std::endl;
          eventList.push_back(this->fanOutPins_[i]->get_gate());
        }
      }
    }
  }
}