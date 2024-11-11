// AuthorCard.jsx

import React from 'react';
// avatar, authorLink
export const AuthorCard = ({ name, ava = 'http://localhost:3001/img/logo2.jpg', authorLink }) => {
  return (
    <div style={styles.card}>
      <div style={styles.avatarContainer}>

        <img src={ava} alt={name} style={styles.avatar} />
      </div>
      <div style={styles.info}>
        <h3 style={styles.name}>
          <a href={authorLink} target="_blank" rel="noopener noreferrer" style={styles.link}>
            {name}
          </a>
        </h3>
      </div>
    </div>
  );
};

const styles = {
  card: {
    display: 'flex',
    alignItems: 'left',
    padding: '20px',
    borderRadius: '8px',
    border: '1px solid #eee',
    backgroundColor: '#502f52',
    marginTop: '20px',
  },
  avatarContainer: {
    marginRight: '20px',
  },
  avatar: {
    width: '60px',
    height: '60px',
    borderRadius: '50%',
    objectFit: 'cover',
  },
  info: {
    maxWidth: '500px',
  },
  name: {
    margin: '0',
    fontSize: '1.5rem',
    fontWeight: 'bold',
  },
  link: {
    color: '#007bff',
    textDecoration: 'none',
  },
  bio: {
    fontSize: '1rem',
    color: '#555',
  },
  links: {
    marginTop: '10px',
  },
  link: {
    color: '#007bff',
    textDecoration: 'none',
    marginRight: '15px',
    fontSize: '1rem',
  },
};
