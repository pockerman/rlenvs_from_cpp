from fastapi import APIRouter
from fastapi import WebSocket, WebSocketDisconnect
from fastapi.responses import HTMLResponse
import numpy as np
import cv2
import json
import asyncio
ws_router = APIRouter(prefix="/ws", tags=["camera"])


html = """
<!DOCTYPE html>
<html>
    <head>
        <title>Chat</title>
    </head>
    <body>
        <h1>WebSocket Chat</h1>
        <form action="" onsubmit="sendMessage(event)">
            <input type="text" id="messageText" autocomplete="off"/>
            <button>Send</button>
        </form>
        <ul id='messages'>
        </ul>
        <script>
            var ws = new WebSocket("ws://localhost:8000/api/ws/text-feed-ws");
            ws.onmessage = function(event) {
                var messages = document.getElementById('messages')
                var message = document.createElement('li')
                var content = document.createTextNode(event.data)
                message.appendChild(content)
                messages.appendChild(message)
            };
            function sendMessage(event) {
                var input = document.getElementById("messageText")
                ws.send(input.value)
                input.value = ''
                event.preventDefault()
            }
        </script>
    </body>
</html>
"""


@ws_router.get("/text-feed")
async def get():
    return HTMLResponse(html)


@ws_router.websocket("/text-feed-ws")
async def websocket_endpoint(websocket: WebSocket):
    await websocket.accept()
    while True:
        data = await websocket.receive_text()
        await websocket.send_text(f"Message text was: {data}")


def draw_label(img, text, pos, bg_color):
   font_face = cv2.FONT_HERSHEY_SIMPLEX
   scale = 0.4
   color = (0, 0, 0)
   thickness = cv2.FILLED
   margin = 2
   txt_size = cv2.getTextSize(text, font_face, scale, thickness)

   end_x = pos[0] + txt_size[0][0] + margin
   end_y = pos[1] - txt_size[0][1] - margin

   cv2.rectangle(img, pos, (end_x, end_y), bg_color, thickness)
   cv2.putText(img, text, pos, font_face, scale, color, 1, cv2.LINE_AA)


@ws_router.websocket("/camera-feed-ws")
async def websocket_endpoint(websocket: WebSocket):
    # listen for connections
    await websocket.accept()
    #count = 1
    print("Accepted connection...")
    try:
        while True:
            contents = await websocket.receive_bytes()
            arr = np.frombuffer(contents, np.uint8)
            frame = cv2.imdecode(arr, cv2.IMREAD_UNCHANGED)
            #cv2.imshow('frame', frame)
            cv2.waitKey(1)

            draw_label(img=frame, text="Iam watching you", pos=(20,20), bg_color=(255,0,0))
            ret, buffer = cv2.imencode('.png', frame)
            #cv2.imshow('preview', frame)

            data_sent = await websocket.send_json({"text": "Iam watching you"})
            #data_sent = await websocket.send(json.dumps({"text": "Iam watching you"}))
            await asyncio.sleep(0.5)
            #cv2.imwrite("frame%d.png" % count, frame)
            #count += 1
    except WebSocketDisconnect:
        cv2.destroyWindow("frame")
        print("Client disconnected")