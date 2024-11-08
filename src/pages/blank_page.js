// Post.js
import React, { useEffect, useState } from 'react';

function Post() {
    const [postData, setPostData] = useState({ author: '', text: '' });

    // Set a pageId in localStorage if it doesn't already exist
    useEffect(() => {
        if (!localStorage.getItem('pageId')) {
            localStorage.setItem('pageId', 1); // You can set this to any unique identifier
        }
    }, []);

    // Fetch data from the server when the component mounts
    useEffect(() => {
        const fetchPostData = async () => {
            const pageId = localStorage.getItem('pageId'); // Retrieve pageId from localStorage

            try {
                const response = await fetch('0.0.0.0:8080/page/1', {
                    method: 'GET',
                    headers: {
                        'Content-Type': 'application/json'
                    }// Send pageId in the request body
                });
                const data = await response.json();
                
                // Set the fetched data in the component's state
                setPostData({
                    author: data.author,
                    text: data.text
                });
            } catch (error) {
                console.error('Error fetching post data:', error);
            }
        };

        fetchPostData();
    }, []);

    return (
        <div style={styles.postContainer}>
            <div style={styles.author}>{postData.author}</div>
            <div style={styles.text}>{postData.text}</div>
        </div>
    );
}

// Inline styles
const styles = {
    postContainer: {
        maxWidth: '600px',
        margin: 'auto',
        background: '#222',
        padding: '20px',
        borderRadius: '8px',
        color: '#fff',
        fontFamily: 'Arial, sans-serif'
    },
    author: {
        fontWeight: 'bold',
        color: '#29d88d',
        marginBottom: '10px'
    },
    text: {
        fontSize: '1.1rem',
        margin: '20px 0'
    }
};

export default Post;
