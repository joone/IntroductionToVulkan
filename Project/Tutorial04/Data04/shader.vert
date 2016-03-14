// Copyright 2016 Intel Corporation All Rights Reserved
// 
// Intel makes no representations about the suitability of this software for any purpose.
// THIS SOFTWARE IS PROVIDED ""AS IS."" INTEL SPECIFICALLY DISCLAIMS ALL WARRANTIES,
// EXPRESS OR IMPLIED, AND ALL LIABILITY, INCLUDING CONSEQUENTIAL AND OTHER INDIRECT DAMAGES,
// FOR THE USE OF THIS SOFTWARE, INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PROPRIETARY
// RIGHTS, AND INCLUDING THE WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// Intel does not assume any responsibility for any errors which may appear in this software
// nor any responsibility to update it.

#version 430

layout(location = 0) in vec4 i_Position;
layout(location = 1) in vec4 i_Color;

layout(location = 0) out vec4 v_Color;

void main() {
    gl_Position = i_Position;
    v_Color = i_Color;
}
