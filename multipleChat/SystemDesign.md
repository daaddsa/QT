# 多人即时通讯系统设计方案 (实验3)

## 1. 项目概述

本项目是一个基于 **C/S (客户端/服务器)** 架构的多人即时通讯系统。系统由 **客户端 (multipleChat)** 和 **服务端 (chatServer)** 两部分组成。
设计目标是实现一个轻量级、核心功能完整的聊天室，重点在于网络连接管理、消息传输与分发、以及系统运行状态的日志监控。

### 1.1 核心功能
*   **服务端 (chatServer)**:
    *   监听指定端口，接受客户端连接。
    *   维护当前在线用户列表。
    *   处理用户登录（用户名注册）。
    *   **消息广播**：将一个用户发送的消息转发给所有其他在线用户。
    *   **状态同步**：向所有用户广播新用户上线、用户下线通知。
    *   **日志系统**：在 UI 上实时显示服务器运行状态（如：新连接、断开、消息转发等）。
*   **客户端 (multipleChat)**:
    *   连接服务器（指定 IP 和端口）。
    *   用户登录（输入用户名）。
    *   **群聊功能**：发送文本消息，接收并显示来自他人的消息。
    *   **用户列表**：实时更新当前在线的用户列表。
    *   **日志显示**：显示连接状态、发送/接收数据的详细日志。

---

## 2. 系统架构

### 2.1 技术栈
*   **开发框架**: Qt 6.9.2 (C++)
*   **网络模块**: `QTcpServer` (服务端), `QTcpSocket` (客户端/服务端)
*   **数据格式**: JSON (`QJsonDocument`, `QJsonObject`)
*   **通信协议**: 基于 TCP 的 JSON 消息协议 (以换行符 `\n` 为消息分隔符)

### 2.2 模块划分

#### 2.2.1 客户端 (multipleChat)
*   **MainWindow**: 主界面，包含聊天记录显示框、输入框、在线用户列表、日志窗口。
*   **LoginDialog** (可选): 登录对话框，用于输入服务器地址和用户名。
*   **ChatClient (网络层)**: 封装 `QTcpSocket`，负责：
    *   建立/断开连接。
    *   发送 JSON 消息 (Login, Message)。
    *   接收并解析 JSON 消息，分发给 UI。

#### 2.2.2 服务端 (chatServer)
*   **MainWindow**: 服务端管理界面，主要用于显示运行日志和控制服务启停。
*   **ChatServer (核心逻辑)**: 继承自 `QTcpServer`，负责：
    *   `incomingConnection`: 处理新连接。
    *   `broadcast`: 消息群发。
*   **ServerWorker (客户端代理)**: 继承自 `QObject`，对应每一个连接的客户端，负责：
    *   维护该连接的 Socket。
    *   处理该连接的数据收发。
    *   存储客户端信息（如用户名）。

---

## 3. 通信协议设计 (JSON)

所有消息均以 JSON 对象格式传输，并以换行符 `\n` 结尾，以解决 TCP 粘包问题。

### 3.1 消息类型 (`type` 字段)

#### 1. 登录 (Login)
*   **方向**: Client -> Server
*   **内容**:
    ```json
    {
        "type": "login",
        "username": "张三"
    }
    ```

#### 2. 登录响应 / 用户列表 (UserList)
*   **方向**: Server -> Client (登录成功后发送)
*   **内容**:
    ```json
    {
        "type": "userlist",
        "userlist": ["张三", "李四", "王五"]
    }
    ```

#### 3. 发送消息 (Message)
*   **方向**: Client -> Server
*   **内容**:
    ```json
    {
        "type": "message",
        "text": "大家好！"
    }
    ```

#### 4. 广播消息 (Message)
*   **方向**: Server -> Client (转发给所有人)
*   **内容**:
    ```json
    {
        "type": "message",
        "sender": "张三",
        "text": "大家好！"
    }
    ```

#### 5. 新用户上线通知 (NewUser)
*   **方向**: Server -> Client (广播给其他人)
*   **内容**:
    ```json
    {
        "type": "newuser",
        "username": "王五"
    }
    ```

#### 6. 用户下线通知 (UserDisconnected)
*   **方向**: Server -> Client (广播给其他人)
*   **内容**:
    ```json
    {
        "type": "userdisconnected",
        "username": "王五"
    }
    ```

---

## 4. 数据流程

1.  **连接阶段**:
    *   Client 发起 `connectToHost`。
    *   Server `incomingConnection` 触发，创建 `ServerWorker`。
    *   Server UI Log: "New connection from IP:..."

2.  **登录阶段**:
    *   Client 发送 `login` JSON。
    *   Server 接收并记录用户名，回复 `userlist` 给该用户。
    *   Server 向其他所有用户广播 `newuser`。
    *   Client UI: 进入聊天界面，显示当前用户列表。

3.  **聊天阶段**:
    *   Client A 发送 `message`。
    *   Server 接收，附加 `sender` 信息。
    *   Server 调用 `broadcast` 将消息发送给所有在线 Client (包括 A 自己，或者 A 自己本地显示)。
    *   Client 收到消息，追加到聊天记录框。

4.  **断开阶段**:
    *   Client 断开 socket。
    *   Server 检测到 `disconnected` 信号。
    *   Server 移除列表中的 Worker。
    *   Server 广播 `userdisconnected` 给剩余用户。

---

## 5. 开发计划 (Todo)

1.  **服务端开发 (chatServer)**
    *   [ ] 完成 `ServerWorker` 类：实现 JSON 数据收发。
    *   [ ] 完成 `ChatServer` 类：实现客户端管理与广播逻辑。
    *   [ ] 完成服务端 UI：显示日志，启动/停止监听。
2.  **客户端开发 (multipleChat)**
    *   [ ] 完成登录 UI (输入用户名、IP)。
    *   [ ] 完成主聊天 UI (列表、输入、显示)。
    *   [ ] 实现网络逻辑：处理登录响应、消息接收、用户列表更新。
3.  **联调与测试**
    *   [ ] 启动 Server。
    *   [ ] 启动多个 Client 实例进行对话测试。
    *   [ ] 验证新用户加入和退出时的列表刷新。

---

## 7. UI 详细设计

### 7.1 服务器端 (chatServer)

服务器端界面主要用于管理员监控系统运行状态，只需一个主窗口。

#### 页面：主控制台 (MainWindow)
*   **功能目标**: 控制服务器启停，查看实时日志。
*   **控件布局**:
    1.  **控制区 (Top)**:
        *   `QLabel`: "监听端口:"
        *   `QSpinBox`: 设置端口号 (默认 1234)。
        *   `QPushButton`: "启动服务器" (点击后变更为 "停止服务器")。
    2.  **日志区 (Center)**:
        *   `QTextBrowser` 或 `QPlainTextEdit`: 只读，显示服务器的所有活动日志（如 "Client A connected", "Forwarded message..."）。
    3.  **状态栏 (Bottom)**:
        *   `QLabel`: 显示当前在线人数。

### 7.2 客户端 (multipleChat)

客户端包含两个主要交互界面：登录连接界面和聊天主界面。

#### 页面 1：登录对话框 (LoginDialog / Initial View)
*   **功能目标**: 获取连接信息和用户身份。
*   **控件布局**:
    *   `QLabel` + `QLineEdit`: "服务器地址" (默认 127.0.0.1)。
    *   `QLabel` + `QSpinBox`: "端口号" (默认 1234)。
    *   `QLabel` + `QLineEdit`: "用户昵称" (必填)。
    *   `QPushButton`: "登录/连接"。
    *   *注：此界面可以在程序启动时作为模态对话框弹出，或者作为 MainWindow 的初始状态（使用 `QStackedWidget` 切换）。*

#### 页面 2：聊天主窗口 (MainWindow)
*   **功能目标**: 发送/接收消息，查看在线用户。
*   **控件布局**:
    *   **左侧 (聊天区)**:
        *   `QTextBrowser` 或 `QListWidget`: 显示历史聊天记录 (只读)。建议使用 `QTextBrowser` 支持简单的富文本（区分自己和他人的消息颜色）。
        *   `QLineEdit`: 消息输入框。
        *   `QPushButton`: "发送" 按钮。
    *   **右侧 (用户列表区)**:
        *   `QLabel`: 标题 "在线用户"。
        *   `QListWidget`: 显示当前在线的所有用户昵称。
    *   **底部 (可选日志区)**:
        *   `QTextEdit`: (只读) 显示底层网络日志（如 "Connected to host", "JSON received..."），便于调试。

