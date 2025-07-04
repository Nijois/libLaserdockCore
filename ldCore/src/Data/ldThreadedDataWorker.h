/**
    libLaserdockCore
    Copyright(c) 2018 Wicked Lasers

    This file is part of libLaserdockCore.

    libLaserdockCore is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libLaserdockCore is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libLaserdockCore.  If not, see <https://www.gnu.org/licenses/>.
**/

#ifndef LDTHREADEDDATAWORKER_H
#define LDTHREADEDDATAWORKER_H

#include <QtCore/QElapsedTimer>
#include <QtCore/QMutex>
#include <QtCore/QObject>

class ldFrameBuffer;
class ldHardwareBatch;
class ldSimulatorEngine;
class ldAbstractHardwareManager;

/** Internal data worker class. Shouldn't be used in any external code */
class ldThreadedDataWorker : public QObject
{
    Q_OBJECT

public:
    explicit ldThreadedDataWorker(ldFrameBuffer *frameBuffer,
                                  ldSimulatorEngine *simulatorEngine,
                                  ldHardwareBatch *hardwareBatch
                        );
    ~ldThreadedDataWorker();

public slots:
    void startProcess();
    void stopProcess();

    void run();

    bool isActiveTransfer() const;
    void setActiveTransfer(bool active);
    void setSimulatorEnabled(bool enabled);

signals:
    void startRun();
    void activeChanged(bool active);
    void finished();

private:
    // thread safe
    std::atomic<bool> m_isRunning = {false};
    std::atomic<bool> m_isActive = {false};
    std::atomic<bool> m_isSimulatorEnabled = {true};

    int m_simulatedBufferFullCount = 0;
    QElapsedTimer m_simTimer;
    qint64 m_lastMs = 0;

    ldSimulatorEngine* m_simulatorEngine;

    ldHardwareBatch * m_hardwareBatch = nullptr;

    ldFrameBuffer *m_frameBuffer;
};

#endif /* LDTHREADEDDATAWORKER_H */
