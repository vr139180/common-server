// Copyright 2021 common-server Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef __CMSWORLD_CONST_H__
#define __CMSWORLD_CONST_H__

//region��һ��cell�ı߳� m
#define REGION_CELL_SIZE		30
//�����ܿ�������Զ������3��cell�ĳ��ȣ�90m
#define PLAYER_EYESEE_CELL_MAX	3

//�����ٶ� 1.5m/s
#define GAME_PLAYER_WALKSPEED	1.5
//�ܲ��ٶ� 2.5m/s
#define GAME_PLAYER_RUNSPEED	2.5

//-------------------------------------------------------
//��ͼ������Ҫ��
//���������30m��������
//��ͼ��ԭ��(0,0)�ڵ�ͼ�����½�
//��ͼ�и�ɵ�region�ĳ��������30m��������
//��ͼ�ָ�����ĵط����������Խ����Ե���㴦��չ3��cell 90m��
//game service����������Э�飬���붨��optional Location3D role_pos = 1
//-------------------------------------------------------

#endif //__CMSWORLD_CONST_H__
