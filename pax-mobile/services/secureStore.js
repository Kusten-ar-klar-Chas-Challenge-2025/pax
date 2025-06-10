import * as SecureStore from "expo-secure-store"

// Spara vad som helst till SecureStore - key och value
export const saveToSecureStore = async (key , value) => {
    try {
        await SecureStore.setItemAsync(key, value);
    } catch (error) {
        console.error("Gick ej att spara till SecureStore", error)
    }
};

// Hämta från SecureStore - key
export const getFromSecureStore = async (key) => {
    try {
        return await SecureStore.getItemAsync(key);
    } catch (error) {
        console.error("Gick ej att hämta från SecureStore", error);
        return null;
    }
};

// Tar bort från SecureStore - key
export const deleteFromSecureStore = async (key) => {
    try {
        await SecureStore.deleteItemAsync(key);
    } catch (error) {
        console.error("Gick ej att ta bort från SecureStore", error);
    }
};