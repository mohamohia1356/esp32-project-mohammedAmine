import React, { useState, useEffect } from 'react';
import { Text, View, StyleSheet, Switch } from 'react-native';
import { getDatabase, ref, onValue, set } from "firebase/database";
import { db } from './config'; // تأكد من أن ملف config.js يحتوي على إعدادات Firebase الصحيحة

function App(): React.JSX.Element {
  // تحديد نوع الكائن كـ Record<string, number> لتجنب الخطأ
  const [controls, setControls] = useState<Record<string, number>>({
    ServoMotor: 0,
    UltraSonic: 0,
    WhiteLed1: 0,
    WhiteLed2: 0,
    WhiteLed3: 0
  });

  useEffect(() => {
    const dbRef = ref(db); // جلب جميع البيانات من Firebase
    const unsubscribe = onValue(dbRef, (snapshot) => {
      const data = snapshot.val();
      if (data) {
        setControls({
          ServoMotor: data.ServoMotor || 0,
          UltraSonic: data.UltraSonic || 0,
          WhiteLed1: data.WhiteLed1 || 0,
          WhiteLed2: data.WhiteLed2 || 0,
          WhiteLed3: data.WhiteLed3 || 0,
        });
      }
    }, (error) => {
      console.error("Error fetching data:", error);
    });

    return () => unsubscribe();
  }, []);

  const toggleControl = async (key: string) => {
    try {
      const newValue = controls[key] === 0 ? 1 : 0;
      setControls((prevControls) => ({ ...prevControls, [key]: newValue })); // تحديث الواجهة فورًا
      const dbRef = ref(db, key);
      await set(dbRef, newValue);
      console.log(`${key} toggled successfully!`);
    } catch (error) {
      console.error(`Error toggling ${key}:`, error);
    }
  };

  return (
    <View style={styles.container}>
      {Object.keys(controls).map((key) => (
        <View key={key} style={styles.row}>
          <Text>{key}: {controls[key] ? "On" : "Off"}</Text>
          <Switch value={controls[key] === 1} onValueChange={() => toggleControl(key)} />
        </View>
      ))}
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#f5f5f5',
  },
  row: {
    flexDirection: 'row',
    alignItems: 'center',
    padding: 20,
  },
});

export default App;
