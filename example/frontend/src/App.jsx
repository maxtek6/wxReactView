import { useState } from 'react'
import './App.css'

function App() {
  const [text, setText] = useState('');
  const [loading, setLoading] = useState(false);

  const handleSend = async () => {
    setLoading(true);
    try {
      await fetch('https://wxreactview.ipc/api/EchoMessage', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({ message: text }),
      });

    } catch (err) {
      console.error('POST request failed:', err);
    }
    setLoading(false);
  };

  return (
    <>
      <h1>Demo</h1>
      <div className="card">
        <input
          type="text"
          value={text}
          onChange={(e) => setText(e.target.value)}
          placeholder="Enter message"
        />
        <button onClick={handleSend} disabled={loading}>
          Send
        </button>
      </div>
    </>
  );
}

export default App;