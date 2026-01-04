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

## 7. UI 详细设计 (增强版)

本系统致力于打造一个完整的即时通讯体验，UI 设计参考现代 IM 软件（如 QQ、微信）的布局风格。

### 7.1 客户端 (multipleChat)

客户端采用多窗口与主面板结合的设计模式。

#### 页面 1：登录与注册 (LoginWidget)
*   **功能**: 用户身份验证与新用户注册。
*   **实现方式**: 使用 `QStackedWidget` 在登录页和注册页之间切换。
*   **控件布局**:
    *   **登录页 (Index 0)**:
        *   `QLabel`: Logo 或标题 "MultipleChat"。
        *   `QLineEdit` (txtAccount): 账号输入框 (Placeholder: "请输入账号").
        *   `QLineEdit` (txtPassword): 密码输入框 (EchoMode: Password).
        *   `QLineEdit` (txtServerIP): 服务器地址 (默认 127.0.0.1, 可折叠或放置于设置中).
        *   `QPushButton` (btnLogin): "登录" 按钮.
        *   `QPushButton` (btnToRegister): "注册账号" 链接/按钮.
    *   **注册页 (Index 1)**:
        *   `QLabel`: 标题 "新用户注册"。
        *   `QLineEdit`: 账号 (必填).
        *   `QLineEdit`: 昵称 (必填).
        *   `QLineEdit`: 密码 (必填).
        *   `QLineEdit`: 确认密码.
        *   `QPushButton` (btnRegister): "立即注册".
        *   `QPushButton` (btnBackToLogin): "返回登录".

#### 页面 2：主界面 (MainWindow)
*   **功能**: 好友管理、群组管理、发起聊天。
*   **布局风格**: 侧边栏导航 + 内容列表 + (可选) 右侧聊天预览。推荐采用 **类似 QQ 的长条形面板** 设计。
*   **控件布局**:
    *   **顶部 (Top Area)**:
        *   `QLabel` (lblAvatar): 用户头像.
        *   `QLabel` (lblNickname): 用户昵称.
        *   `QLineEdit` (txtSearch): 搜索好友/群组.
    *   **中部 (Tab Content)**:
        *   `QTabWidget` 或 `QToolBox`:
            *   **Tab 1: 好友列表 (Contacts)**: 使用 `QTreeWidget` 实现分组显示 (如 "我的好友", "黑名单").
            *   **Tab 2: 群组列表 (Groups)**: 显示已加入的群聊.
            *   **Tab 3: 最近会话 (Recent)**: 显示最近聊天的对象.
    *   **底部 (Bottom Toolbar)**:
        *   `QPushButton` (btnAdd): 打开 "查找/添加好友" 窗口.
        *   `QPushButton` (btnCreateGroup): 打开 "创建群组" 窗口.
        *   `QPushButton` (btnSettings): 系统设置.

#### 页面 3：聊天窗口 (ChatWindow)
*   **功能**: 核心聊天交互 (支持单聊和群聊).
*   **交互逻辑**: 双击主界面的好友或群组时弹出此窗口。
*   **控件布局**:
    *   **标题栏**: 显示聊天对象名称 (用户昵称 或 群名).
    *   **消息展示区 (Center)**:
        *   `QListWidget` 或 `QWebEngineView`: 用于显示消息气泡.
        *   **左侧气泡**: 对方发送的消息.
        *   **右侧气泡**: 自己发送的消息.
        *   **时间戳**: 间隔显示.
    *   **输入工具栏 (Toolbar)**:
        *   `QPushButton`: 表情、图片、文件发送按钮 (扩展功能).
    *   **输入区 (Bottom)**:
        *   `QTextEdit`: 多行文本输入.
        *   `QPushButton` (btnSend): "发送" (支持 Ctrl+Enter).

#### 页面 4：查找与添加 (SearchDialog)
*   **功能**: 搜索并添加好友.
*   **控件**:
        *   `QLineEdit`: 输入账号/昵称关键词.
        *   `QPushButton`: "搜索".
        *   `QListWidget`: 显示搜索结果列表 (包含 "添加" 按钮).

### 7.2 服务器端 (chatServer)

服务端作为管理控制台，重点在于数据的可视化监控。

#### 页面：服务器管理控制台 (ServerMainWindow)
*   **Tab 1: 运行监控 (Monitor)**
    *   `QTextBrowser`: 实时系统日志 (连接、断开、错误).
    *   `QLCDNumber` 或 `QLabel`: 当前在线人数、今日消息总数.
    *   `QPushButton`: 启动/停止服务.
*   **Tab 2: 用户管理 (UserManager)**
    *   `QTableWidget`: 显示所有注册用户表 (ID, 账号, 昵称, 注册时间, 是否在线).
    *   **操作**: 支持强制下线、封禁账号 (扩展功能).
*   **Tab 3: 系统配置 (Config)**
    *   `QFormLayout`: 设置监听端口、最大连接数、数据库路径等.

