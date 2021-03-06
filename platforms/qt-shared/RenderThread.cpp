/*
 * Gearsystem - Sega Master System / Game Gear Emulator
 * Copyright (C) 2013  Ignacio Sanchez

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/
 *
 */

#include "RenderThread.h"
#include "GLFrame.h"
#include "Emulator.h"

RenderThread::RenderThread(GLFrame* pGLFrame) : QThread(), m_pGLFrame(pGLFrame)
{
    m_bPaused = false;
    m_bDoRendering = true;
    m_pFrameBuffer = new GS_Color[GS_RESOLUTION_MAX_WIDTH * GS_RESOLUTION_MAX_HEIGHT];
    m_iWidth = 0;
    m_iHeight = 0;
    InitPointer(m_pEmulator);
    m_bFiltering = false;
    for (int i=0; i<3; i++)
        m_GBTexture[i] = 0;
}

RenderThread::~RenderThread()
{
}

void RenderThread::ResizeViewport(const QSize &size, int pixel_ratio)
{
    m_iWidth = size.width() * pixel_ratio;
    m_iHeight = size.height() * pixel_ratio;
}

void RenderThread::Stop()
{
    m_bDoRendering = false;
}

void RenderThread::Pause()
{
    m_bPaused = true;
}

void RenderThread::Resume()
{
    m_bPaused = false;
}

bool RenderThread::IsRunningEmulator()
{
    return m_bDoRendering;
}

void RenderThread::SetEmulator(Emulator* pEmulator)
{
    m_pEmulator = pEmulator;
}

void RenderThread::run()
{
    m_pGLFrame->makeCurrent();

    Init();

    m_Timer.restart();

    while (m_bDoRendering)
    {
        if (m_pGLFrame->parentWidget()->window()->isVisible())
        {
            m_pGLFrame->makeCurrent();

            if (m_bPaused)
            {
                msleep(200);
            }
            else
            {
                m_pEmulator->RunToVBlank(m_pFrameBuffer);
                RenderFrame();
            }

            if (!m_pEmulator->IsAudioEnabled())
            {
                qint64 elapsed = m_Timer.nsecsElapsed();

                if (elapsed < 16000000)
                {
                    usleep((16000000 - elapsed) / 1000);
                }

                m_Timer.restart();
            }

            m_pGLFrame->swapBuffers();
        }
    }

    SafeDeleteArray(m_pFrameBuffer);
    glDeleteTextures(3, m_GBTexture);
}

void RenderThread::Init()
{
    for (int y = 0; y < GS_RESOLUTION_MAX_HEIGHT; ++y)
    {
        for (int x = 0; x < GS_RESOLUTION_MAX_WIDTH; ++x)
        {
            int pixel = (y * GS_RESOLUTION_MAX_WIDTH) + x;
            m_pFrameBuffer[pixel].red = m_pFrameBuffer[pixel].green =
                    m_pFrameBuffer[pixel].blue = 0x00;
            m_pFrameBuffer[pixel].alpha = 0xFF;
        }
    }

#ifndef __APPLE__
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        Log("GLEW Error: %s\n", glewGetErrorString(err));
    }
    Log("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif

    SetupTexture((GLvoid*) m_pFrameBuffer);
}

void RenderThread::SetupTexture(GLvoid* data)
{
    glGenTextures(3, m_GBTexture);

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, m_GBTexture[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GS_RESOLUTION_GG_WIDTH, GS_RESOLUTION_GG_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*) data);
    glBindTexture(GL_TEXTURE_2D, m_GBTexture[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GS_RESOLUTION_SMS_WIDTH, GS_RESOLUTION_SMS_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*) data);
    glBindTexture(GL_TEXTURE_2D, m_GBTexture[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GS_RESOLUTION_SMS_WIDTH, GS_RESOLUTION_SMS_HEIGHT_EXTENDED, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*) data);
}

void RenderThread::RenderFrame()
{
    GS_RuntimeInfo runtime_info;
    m_pEmulator->GetRuntimeInfo(runtime_info);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    switch (runtime_info.screen_height)
    {
        case GS_RESOLUTION_GG_HEIGHT:
            glBindTexture(GL_TEXTURE_2D, m_GBTexture[0]);
            break;
        case GS_RESOLUTION_SMS_HEIGHT:
            glBindTexture(GL_TEXTURE_2D, m_GBTexture[1]);
            break;
        case GS_RESOLUTION_SMS_HEIGHT_EXTENDED:
            glBindTexture(GL_TEXTURE_2D, m_GBTexture[2]);
            break;

    }

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, runtime_info.screen_width, runtime_info.screen_height,
            GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*) m_pFrameBuffer);
    if (m_bFiltering)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }
    RenderQuad(m_iWidth, m_iHeight, false);
}

void RenderThread::RenderQuad(int viewportWidth, int viewportHeight, bool mirrorY)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (mirrorY)
        gluOrtho2D(0, viewportWidth, 0, viewportHeight);
    else
        gluOrtho2D(0, viewportWidth, viewportHeight, 0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, viewportWidth, viewportHeight);

    glBegin(GL_QUADS);
    glTexCoord2d(0.0, 0.0);
    glVertex2d(0.0, 0.0);
    glTexCoord2d(1.0, 0.0);
    glVertex2d(viewportWidth, 0.0);
    glTexCoord2d(1.0, 1.0);
    glVertex2d(viewportWidth, viewportHeight);
    glTexCoord2d(0.0, 1.0);
    glVertex2d(0.0, viewportHeight);
    glEnd();
}

void RenderThread::SetBilinearFiletering(bool enabled)
{
    m_bFiltering = enabled;
}
