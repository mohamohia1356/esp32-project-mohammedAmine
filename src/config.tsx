// Import the functions you need from the SDKs you need
import { initializeApp } from "firebase/app";
import { getAnalytics } from "firebase/analytics";
import { getDatabase } from "firebase/database";

// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

// Your web app's Firebase configuration
// For Firebase JS SDK v7.20.0 and later, measurementId is optional
const firebaseConfig = {
  apiKey: "AIzaSyCQ1FoNhkuH3TRDUXxGIf4knSNQ-_JVYoU",
  authDomain: "esp32wroom-30937.firebaseapp.com",
  databaseURL: "https://esp32wroom-30937-default-rtdb.firebaseio.com",
  projectId: "esp32wroom-30937",
  storageBucket: "esp32wroom-30937.firebasestorage.app",
  messagingSenderId: "279753923076",
  appId: "1:279753923076:web:1a2a74b61f728389a4b351",
  measurementId: "G-C9L75LN69M"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const analytics = getAnalytics(app);
// Initialize Realtime Database and get a reference to the service
export const db = getDatabase(app);