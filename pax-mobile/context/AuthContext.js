import React, { createContext, useContext, useEffect, useState } from 'react'
import { saveToStorage, getFromStorage, deleteFromStorage } from "../services/webCompatibleSecureStore"
import { useUser } from './UserContext';

const AuthContext = createContext();

export const AuthProvider = ({ children }) => {
    const [token, setToken] = useState(null);
    const [isLoading, setIsLoading] = useState(true);
    const { saveUserData, clearUser } = useUser();

    useEffect(() => {
        const loadToken = async () => {
            const storedToken = await getFromStorage('userToken');

            if (storedToken) {
                setToken(storedToken);
            }

            setIsLoading(false);
        };
        loadToken();
    }, []);

    const login = async (newToken) => {
        console.log("Token", newToken);
        await saveToStorage("userToken", newToken);
        setToken(newToken);
    };

    const logout = async () => {
        await deleteFromStorage("userToken");
        setToken(null);
        clearUser();
    }

    return (
        <AuthContext.Provider value={{ token, login, logout, isLoading }}>
            {children}
        </AuthContext.Provider>
    );
};

export const useAuth = () => useContext(AuthContext);