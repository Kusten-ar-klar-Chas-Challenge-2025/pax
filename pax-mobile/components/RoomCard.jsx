import React, { useState, useEffect, useRef } from "react";
import { View, Text, StyleSheet, TouchableOpacity } from "react-native";
import { useTheme } from "../theme/ThemeContext";

const RoomCard = () => {
  const [bookedRoom, setBookedRoom] = useState({ name: "Norra Berget" });
  const [pauseTime, setPauseTime] = useState(0);
  const timerRef = useRef(null);
  const { theme } = useTheme();

  const startPauseTimer = () => {
    if (pauseTime === 0) {
      setPauseTime(15 * 60);
    }
  };

  useEffect(() => {
    if (pauseTime > 0) {
      timerRef.current = setInterval(() => {
        setPauseTime((prev) => {
          if (prev <= 1) {
            clearInterval(timerRef.current);
            return 0;
          }
          return prev - 1;
        });
      }, 1000);
    }

    return () => clearInterval(timerRef.current);
  }, [pauseTime]);

  const cancelBooking = () => {
    setBookedRoom(null);
    setPauseTime(0);
  };

  const formatTime = (secs) => {
    const min = Math.floor(secs / 60);
    const sec = secs % 60;
    return `${min}:${sec.toString().padStart(2, "0")}`;
  };

  const styles = StyleSheet.create({
    card: {
      padding: 20,
      margin: 20,
      marginBottom: 0,
      borderRadius: 12,
      backgroundColor: theme.card,
      elevation: 4,
      alignItems: "center",
    },
    title: {
      fontSize: 20,
      fontWeight: "800",
      marginBottom: 6,
      textAlign: "center",
      color: theme.textPrimary,
      fontFamily: "NunitoSans",
    },
    roomName: {
      fontSize: 16,
      fontWeight: "500",
      marginBottom: 10,
      color: theme.text,
      fontFamily: "NunitoSans",
      color: theme.textPrimary,
      fontStyle: "italic",
    },
    pauseText: {
      fontSize: 26,
      color: theme.text,
      marginBottom: 10,
      fontWeight: "300",
      textAlign: "center",
      fontFamily: "NunitoSans",
      color: theme.textPrimary,
    },
    buttonRow: {
      flexDirection: "row",
      justifyContent: "space-between",
      width: 320,
      marginTop: 10,
    },
    button: {
      backgroundColor: theme.buttonColor,
      padding: 12,
      flex: 1,
      marginHorizontal: 5,
      borderRadius: 8,
      alignItems: "center",
    },
    cancelButton: {
      backgroundColor: "red",
    },
    buttonText: {
      color: "#fff",
      fontWeight: "600",
      fontFamily: "NunitoSans",
    },
  });

  return (
    <View style={styles.card}>
      {bookedRoom ? (
        <>
          <Text style={styles.title}>Ditt bokade rum är: </Text>
          <Text style={styles.roomName}>{bookedRoom.name}</Text>
          {pauseTime > 0 && (
            <Text style={styles.pauseText}>
              Ditt rum är pausat i: {formatTime(pauseTime)}
            </Text>
          )}
          <View style={styles.buttonRow}>
            <TouchableOpacity style={styles.button} onPress={startPauseTimer}>
              <Text style={styles.buttonText}>Pausa bokning</Text>
            </TouchableOpacity>
            <TouchableOpacity
              style={[styles.button, styles.cancelButton]}
              onPress={cancelBooking}
            >
              <Text style={styles.buttonText}>Avboka</Text>
            </TouchableOpacity>
          </View>
        </>
      ) : (
        <>
          <Text style={styles.title}>Du har inget bokat rum</Text>
        </>
      )}
    </View>
  );
};

export default RoomCard;
