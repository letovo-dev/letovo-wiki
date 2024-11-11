const express = require('express');
const path = require('path');
const cors = require('cors');
const { createProxyMiddleware } = require('http-proxy-middleware');

const app = express();
const PORT = process.env.PORT || 3001;

// Enable CORS for static Docusaurus files (optional, based on your use case)
app.use(cors());

// Serve static files from the Docusaurus build folder
app.use(express.static(path.join(__dirname, 'build')));


app.use('/api', createProxyMiddleware({
  target: 'http://0.0.0.0:8080', // URL of the backend server
  changeOrigin: true,              // Update the origin of the request to the target URL
  pathRewrite: { '^/api': '', '^/blank_page/': '' }     // Remove the /api prefix when forwarding
}));

// Fallback route to serve the Docusaurus index.html for client-side routing
app.get('*', (req, res) => {
  res.sendFile(path.join(__dirname, 'build', 'index.html'));
});

// Start the server
app.listen(PORT, () => {
  console.log(`Server is running on http://localhost:${PORT}`);
});
