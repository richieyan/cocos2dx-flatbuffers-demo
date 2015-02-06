//
//  main.cpp
//  CocosFlatbuffersDemo
//
//  Created by Richie Yan on 2/6/15.
//  Copyright (c) 2015 Richie Yan. All rights reserved.
//

#include <iostream>
#include "flatbuffers/flatbuffers.h"



#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "flatbuffers.h"
#include "flatbuffers/flatbuffers.h"
#include "util.h"

///flatc -c -o ./ GameModel.fbs to generate this file
#include "GameModel_generated.h"

const char* output_file = "/tmp/game_model.data";

void test_write()
{
    using namespace fbs;
    using namespace std;
    using namespace flatbuffers;
    
    flatbuffers::FlatBufferBuilder fbb;

    //create the model builder
    GameModelBuilder gmb(fbb);

    //create struct vector
    SkillModel skill(1,31);//id,level
    vector<const SkillModel*> skillVector;
    skillVector.push_back(&skill);
    auto skills = fbb.CreateVector(skillVector);
    
    //create table vector
    auto ach = CreateAchievementModel(fbb,2,3);//id,step
    vector<Offset<AchievementModel>> achVector;
    achVector.push_back(ach);
    auto achs = fbb.CreateVector(achVector);
    
    //create string value
    auto name = fbb.CreateString("Allen Jobs");
    
    //add value to model builder
    gmb.add_gold(88379);
    gmb.add_level(12);
    gmb.add_username(name);
    //add skills
    gmb.add_skills(skills);
    gmb.add_achievements(achs);
    //create the model
    auto model = gmb.Finish();
    
    //serialize the model
    fbb.Finish(model);
    
    //save the result to file (SaveFile is in util.h)
    flatbuffers::SaveFile(output_file, reinterpret_cast<char *>(fbb.GetBufferPointer()), fbb.GetSize(), true);
    
}

void test_read(){
    using namespace std;
    using namespace flatbuffers;
    using namespace fbs;

    FlatBufferBuilder fbb;
    
    string binaryfile;
    bool ok = flatbuffers::LoadFile(output_file, true, &binaryfile);
    flatbuffers::Verifier tex_verify(fbb.GetBufferPointer(), fbb.GetSize());
    
    bool valid = VerifyGameModelBuffer(tex_verify);
    cout << "ok:" << ok  << " verifier:" << valid << endl;
    
    GameModelBuilder* builder = new GameModelBuilder(fbb);
    
    fbb.PushBytes(reinterpret_cast<unsigned char*>(const_cast<char *>(binaryfile.c_str())), binaryfile.size());
    auto model = fbs::GetGameModel(fbb.GetBufferPointer());
    
    auto level = model->level();
    int gold = model->gold();
    auto username = model->username()->c_str();
    
    cout << "name:" << username << " level:" << level << " gold" << gold << endl;
    auto skills = model->skills();
    
    for(int i = 0;i<skills->size();i++){
        auto skill = skills->Get(i);
        cout << "skillId " << skill->skillId() << " level:" << skill->skillLevel() << endl;
    }
    
    auto achs = model->achievements();
    for(int i = 0;i<achs->size();i++){
        auto ach = achs->Get(i);
        cout << "achievement id " << ach->id() << " step:" << ach->step() << endl;
    }
    
    delete builder;
}

int main(int argc, const char * argv[]) {
    test_write();
    test_read();
    
}
